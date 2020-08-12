/*
 * NetworkDevision.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#include <math.h>
#include <stdlib.h>
#include "NetworkDivision.h"
#include "NodeUtils.h"
#include "LinearUtils.h"
#include "LibFuncsHandler.h"

/*
int main(FILE* input, FILE* output){
	return 0;
}
*/
/*
void write_clusters_to_output(Group* O){
}
*/
/*
int* devide_into_two(Network* N, Node* g){

}
*/
/*
int* calculate_s(double* eigen_vector){

}
*/
/*
Node* divide_group(Node* g, int* s){

}
*/

double calc_Qk(Network* N, double* s, Node* g, int n_g){
	double res;
	double * result;

	result = (double*)allocate(n_g * sizeof(double));
	Bhat_multiplication(N, s, result, g, n_g);
	res = dot_product(s, result, n_g);
	free(result);

	return res;
}

void modularity_maximization(Network* N, double* s, Node* g, int n_g){
	int i;
	int k;
	int max_score_index = -1;
	int improve_index = 0;
	double Q0, Qk;
	double delta_Q;
	double Q_diff = - INFINITY;
	double max_score = - INFINITY;
	double* improve;
	int* indices;
	int* unmoved;
	Node* g_p = g;



	/* Initiate unmoved with all the vertices' indexes corresponding to g */
	unmoved = (int*)allocate(n_g * sizeof(int));
	for(i = 0; i < n_g; i++){
		unmoved[i] = g_p->index;
		g_p = g_p->next;
	}

	improve = (double*)allocate(n_g * sizeof(double));
	indices = (int*)allocate(n_g * sizeof(int));

	do{
		for (i = 0; i < n_g; i++){
			Q0 = calc_Qk(N, s, g, n_g);

			for(k = 0; k < n_g; k++){

				if(unmoved[k] != (-1)){
					s[k] = s[k]*(-1);
					Qk = calc_Qk(N, s, g, n_g);
					Q_diff = Qk - Q0;
					if (Q_diff > max_score){
						max_score = Q_diff;
						max_score_index = k;
					}
					s[k] = s[k]*(-1);
				}
			}

			/* Move max_score_index to the other group */
			s[max_score_index] = s[max_score_index]*(-1);

			/* Add max_score_index to indices, which keeps the vertices transferring order */
			indices[i] = max_score_index;

			/* Update improve */
			if (i == 0){
				improve[i] = max_score;
			}
			else{
				improve[i] = improve[i-1] + max_score;
			}

			/* Remove max_score_index from unmoved */
			unmoved[max_score_index] = -1;

		}

		/* Find the largest improvement index */
		for (k = 0; k < n_g; k++){
			if (improve[k] > improve[improve_index]){
				improve_index = k;
			}
		}

		/* Move back all the remaining vertices that do not improve the modularity */
		for(i = n_g - 1; i > improve_index; i--){
			k = indices[i];
			s[k] = s[k]*(-1);
		}

		/* If the best separation occurs when all the vertices switched groups, then nothing actually changed */
		if (improve_index == n_g - 1){
			delta_Q = 0;
		}
		else{
			delta_Q = improve[improve_index];
		}


	}while(delta_Q > 0);

}














