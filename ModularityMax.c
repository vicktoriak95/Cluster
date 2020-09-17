/*
 * ModularityMax.c
 *
 */

#include "ModularityMax.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "LibFuncsHandler.h"
#include "NetworkDivision.h"

void modularity_maximization(Network* N, double* s, Node* g, int n_g, double* row_sums){
	int i = 0;
	int k = 0;
	int max_score_index = -1;
	int improve_index = 0;
	double delta_Q = 0;
	double Q_diff = 0;
	double max_score = 0;
	int* indices = NULL;
	int* unmoved = NULL;
	int cnt = 0;
	int first_score = 1;
	int power_of_2 = n_g;
	int* next_unmoved = NULL;
	Node* g_unmoved_head = g;
	int* temp;
	double* A_sums = NULL;
	int real_k = 0;
	int real_max_score_index = 0;
	double curr_improve = 0;
	double max_improve = -HUGE_VAL;
	double Q_max = 0;
	double Q_0;
	double base_aux_sum = 0;
	double new_aux_sum = 0;

	printf("got into mod_max bitches \n");

	/* Initiate unmoved with all the vertices indexes corresponding to g */
	unmoved = (int*)allocate(n_g * sizeof(int));
	indices = (int*)allocate(n_g * sizeof(int));
	next_unmoved = (int*)allocate(n_g * sizeof(int));
	A_sums = (double*)allocate(n_g * sizeof(double));

	/* Initiating unmoved with g values */
	vector_from_list(unmoved, g, n_g);

	/* Calc Q_0*/
	Q_0 = calc_Qk(N, s, g, n_g, row_sums);

	/* Keep improving while delta_Q > 0 */
	do{
		if (n_g > INT_MAX_POWER_OF_2){
			power_of_2 = INT_MAX_POWER_OF_2;
		}
		/* Every iteration increases the Q of the division and thus we would not visit the same division twice
		 * There are at most (2 ** n_g) divisions */
		infinite_loop_detection(cnt, pow(2, power_of_2));

		/* Calculating Aux sums */
		 A_row_sums(g, N, A_sums, n_g, s);
		 base_aux_sum = aux_sum_score(N, s, g, n_g);

		/* Initiating unmoved with g values */
		/*
		vector_from_list(unmoved, g, n_g);
		*/

		/* Initiating improve variabales */
		max_improve = -HUGE_VAL;
		improve_index = -1;

		/* Making n_g transitions of vertices to improve Q */
		for (i = 0; i < n_g; i++){

			/* Updating next unmoved */
			next_unmoved[i] = g_unmoved_head->index;
			g_unmoved_head = g_unmoved_head->next;

			max_score_index = -1;
			first_score = 1;
			/* Searching for the best node to move among unmoved */
			for(k = 0; k < n_g; k++){
				if(unmoved[k] != (-1)){
					real_k = get_node_value(g, k);
					new_aux_sum = base_aux_sum - 2 * (s[k] * N->deg_vector[real_k]);
					Q_diff = calc_Q_diff(s, k, real_k, N, A_sums[k], new_aux_sum);
					if ((first_score == 1) || (Q_diff > max_score)){
						first_score = 0;
						max_score = Q_diff;
						max_score_index = k;
						real_max_score_index = real_k;
						Q_max = Q_0 + Q_diff;
					}
				}
			}

			/* Update A sums */
			update_A_sums(A_sums, max_score_index, real_max_score_index, N, s ,g);
			base_aux_sum -= 2 * (s[max_score_index] * N->deg_vector[real_max_score_index]);

			/* Move max_score_index to the other group */
			s[max_score_index] = s[max_score_index]*(-1);

			/* Add max_score_index to indices, which keeps the vertices transferring order */
			indices[i] = max_score_index;

			/* Update improve */
			curr_improve += Q_max - Q_0;
			if(max_improve < curr_improve){
				max_improve = curr_improve;
				improve_index = i;
			}

			/* Remove max_score_index from unmoved */
			unmoved[max_score_index] = -1;
			Q_0  = Q_max;
		}

		/* If the best separation occurs when all the vertices switched groups, then nothing actually changed */
		if (improve_index == n_g - 1){
			delta_Q = 0;
		}
		else{
			delta_Q = max_improve;
			/* Move back all the remaining vertices that do not improve the modularity */
			for(i = n_g - 1; i > improve_index; i--){
				k = indices[i];
				s[k] = s[k]*(-1);
			}
		}

		/* Switching unmoved and unmoved next*/
		temp = unmoved;
		unmoved = next_unmoved;
		next_unmoved = temp;
		g_unmoved_head = g;

		/* Updating Q_0 */
		Q_0 = Q_0 + max_improve;

		cnt += 1;
	} while(delta_Q > 0);

	free(unmoved);
	free(indices);
	free(next_unmoved);
}

void update_A_sums(double* A_sums, int k, int real_k, Network* N, double* s, Node* g){
	Node_matrix* row_head = NULL;
	Node* g_head = g;
	int cnt = 0;
	int mat_col_index = 0;
	int g_col_index = 0;
	int vector_index = 0;
	spmat* A = N->A;

	row_head = ((Node_matrix** )A->private)[real_k];

	/* Iterating over row, summing only entries in g */
	while ((row_head != NULL) && (g_head != NULL)){
		infinite_loop_detection(cnt, N->n);

		/* Comparing indices and promoting g_head, mat_head respectively */
		mat_col_index = row_head->col_index;
		g_col_index = g_head->index;
		if (mat_col_index == g_col_index){
			A_sums[vector_index] -= 2 * row_head->value * s[k];
			g_head = g_head->next;
			row_head = (Node_matrix*)row_head->next;
			vector_index += 1;
		}
		else if (mat_col_index > g_col_index){
			g_head = g_head->next;
			vector_index += 1;
		}
		else {
			row_head = (Node_matrix*)row_head->next;
		}
		cnt += 1;
	}
}

double aux_sum_score(Network* N, double* s, Node* g, int n_g){
	int i = 0;
	double aux_sum = 0;
	Node* g_head = g;
	int real_i = 0;

	for(i=0; i<n_g; i++){
		real_i = g_head->index;
		aux_sum += (double)N->deg_vector[real_i] * s[i];
		g_head = g_head->next;
	}
	return aux_sum;
}
