/*
 * ModularityMax.c
 *
 */

#include "ModularityMax.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "LibFuncsHandler.h"
#include "LinearUtils.h"

void modularity_maximization(Network* N, spmat* A, double* s, Node* g, int n_g, double* row_sums){
	int loop_cnt = 0;
	int power_of_2 = n_g;
	int max_improve_index = 0;
	int* indices = NULL;
	int* unmoved = NULL;
	double delta_Q = 0;
	double max_improve = -HUGE_VAL;
	double Q_0;
	double* A_sums = NULL;

	/* Initiate unmoved with all the vertices indexes corresponding to g */
	unmoved = (int*)allocate(n_g * sizeof(int));
	indices = (int*)allocate(n_g * sizeof(int));
	A_sums = (double*)allocate(n_g * sizeof(double));

	/* Calc Q_0*/
	Q_0 = calc_Qk(N, A, s, g, n_g, row_sums);

	/* Keep improving while delta_Q > 0 */
	do{
		if (n_g > INT_MAX_POWER_OF_2){
			power_of_2 = INT_MAX_POWER_OF_2;
		}
		/* Every iteration increases the Q of the division and thus we would not visit the same division twice
		 * There are at most (2 ** n_g) divisions */
		infinite_loop_detection(loop_cnt, pow(2, power_of_2));

		/* Initiating improve variables */
		max_improve = -HUGE_VAL;
		max_improve_index = -1;

		/* Move n_g vertices and find best improve */
		find_best_improve(N, A, g, n_g, A_sums, s, unmoved, &Q_0, indices, &max_improve, &max_improve_index);

		/* If the best separation occurs when all the vertices switched groups, then nothing actually changed */
		if (max_improve_index == n_g - 1){
			delta_Q = EPSILON;
		}
		else{
			delta_Q = max_improve;
			/* Move back all the remaining vertices that do not improve the modularity */
			flip_s(s, indices, n_g, max_improve_index);
		}

		/* Updating Q_0 */
		Q_0 = Q_0 + max_improve;

		loop_cnt += 1;
	} while(delta_Q > EPSILON);

	free(unmoved);
	free(indices);
	free(A_sums);
}

void update_A_sums(double* A_sums, int k, spmat* A, double* s){
	Node_matrix* node = NULL;
	int c = 0;

	node = ((Node_matrix**)(A->private))[k];
	while (node != NULL){
		infinite_loop_detection(c, A->n);
		A_sums[node->col_index] -= 2 * s[k];
		node = node->next;
		c += 1;
	}
}

double calc_sk_aux_sum(Network* N, double* s, Node* g, int n_g){
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

void flip_s(double* s, int* indices, int n_g, int max_improve_index){
	int i = 0;
	int k = 0;
	for(i = n_g - 1; i > max_improve_index; i--){
		k = indices[i];
		s[k] = s[k] * (-1);
	}
}

void find_best_vertex_to_move(Network* N, Node* g, double* s, int n_g, int* unmoved,
		double base_aux_sum, double* A_sums, double* max_diff, int* max_diff_index, int* real_max_diff_index, double* Q_max, double Q_0){
	int k = 0;
	int real_k = 0;
	double new_aux_sum = 0;
	double Q_diff = 0;
	Node* g_pointer = g;

	/* Searching for the best node to move among unmoved */
	for(k = 0; k < n_g; k++){
		if(unmoved[k] != (-1)){
			real_k = g_pointer->index;
			new_aux_sum = base_aux_sum - 2 * (s[k] * N->deg_vector[real_k]);
			Q_diff = calc_Q_diff(s, k, real_k, N, A_sums[k], new_aux_sum);
			if (Q_diff > *max_diff){
				*max_diff = Q_diff;
				*max_diff_index = k;
				*real_max_diff_index = real_k;
				*Q_max = Q_0 + Q_diff;
			}
		}
		g_pointer = g_pointer->next;
	}
}

void find_best_improve(Network* N, spmat* A, Node* g, int n_g, double* A_sums, double* s, int* unmoved, double* Q_0, int* indices, double* max_improve, int* max_improve_index){
	double base_aux_sum = 0;
	int i = 0;
	int max_diff_index = -1;
	double max_diff = -HUGE_VAL;
	int real_max_diff_index = -1;
	double Q_max = -HUGE_VAL;
	double curr_improve = 0;

	/* Calculating Aux sums */
	spmat_mult(A, s, A_sums);
	base_aux_sum = calc_sk_aux_sum(N, s, g, n_g);

	/* Initiating unmoved with g values */
	vector_from_list(unmoved, g, n_g);

	/* Making n_g transitions of vertices to improve Q */
	for (i = 0; i < n_g; i++){

		max_diff_index = -1;
		max_diff = -HUGE_VAL;
		real_max_diff_index = -1;

		/* Searching for the best node to move among unmoved */
		find_best_vertex_to_move(N, g, s, n_g, unmoved, base_aux_sum, A_sums, &max_diff, &max_diff_index, &real_max_diff_index, &Q_max, *Q_0);

		/* Update A sums */
		update_A_sums(A_sums, max_diff_index, A, s);
		base_aux_sum -= 2 * (s[max_diff_index] * N->deg_vector[real_max_diff_index]);

		/* Move max_score_index to the other group */
		s[max_diff_index] = s[max_diff_index]*(-1);

		/* Add max_score_index to indices, which keeps the vertices transferring order */
		indices[i] = max_diff_index;

		/* Update improve */
		curr_improve += Q_max - *Q_0;
		if(*max_improve < curr_improve){
			*max_improve = curr_improve;
			*max_improve_index = i;
		}

		/* Remove max_score_index from unmoved */
		unmoved[max_diff_index] = -1;
		*Q_0 = Q_max;
	}

}

double calc_Qk(Network* N, spmat* A, double* s, Node* g, int n_g, double* row_sums){
	double res = 0;
	double* result = NULL;

	result = (double*)allocate(n_g * sizeof(double));
	Bhat_multiplication(N, A, s, result, g, n_g, row_sums);
	res = dot_product(s, result, n_g);
	free(result);

	return res;
}

double calc_Q_diff(double* s, int k, int real_k, Network* N, double A_sum, double aux_sum){
	double res = 0;
	double deg_k = (double)N->deg_vector[real_k];
	double M = (double)N->M;

	res = -4 * s[k] * (A_sum - ((deg_k / M) * aux_sum)) + 4 * ((deg_k * deg_k) / M);

	return res;
}
