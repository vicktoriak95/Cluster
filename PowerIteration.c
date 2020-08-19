/*
 * PowerIteration.c
 *
 *  Created on: Aug 10, 2020
 *      Author: User
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PowerIteration.h"
#include "LibFuncsHandler.h"
#include "LinearUtils.h"

double* power_iteration(Network* N, double norm, Node* g, int n_g){
	/*double b_prev_const[3] = {0.36283495, 0.56467796, 0.66505637};
	double b_next_const[3] = {0.11767303, 0.5011049, 0.89936061};*/
	double* temp;
	double* b_prev;
	double* b_next;
	double vector_norm = 0;


	/* Initiating b_prev to be random b_0 */
	b_prev = (double*)allocate(n_g * sizeof(double));
	create_random_vector(b_prev, n_g);
	b_next =(double*)allocate(n_g * sizeof(double));
	create_random_vector(b_next, n_g);

	/* TODO: How to assure loop is not infinite */
	/* While vectors diff bigger than epsilon */
	while(close_vectors(b_prev, b_next, n_g) != 0){

		/* Dot product between B\hat[g]_shifted and b_k */

		/* Multiplying B\hat by b_prev and saving result in b_next */
		Bhat_multiplication(N, b_prev, b_next, g, n_g);
		/* Shifting b_next by b_prev and norm*/
		Bhat_shift(b_next, b_prev, norm, n_g);

		/* Normalizing over norm */
		vector_norm = sqrt(dot_product(b_next, b_next, n_g));
		mult_vector_by_scalar(b_next, 1 / vector_norm, n_g);

		temp = b_prev;
		b_prev = b_next;
		b_next = temp;
	}
	free(b_next);
	return b_prev;
}

void create_random_vector(double * vector, int vector_size){
	int i;
	for(i=0; i<vector_size; i++){
		vector[i] = (double)rand();
	}
}

int close_vectors(double* vector_a, double* vector_b, int n){
	int i;
	/* Iterating over vectors, checking if diff of every entry is bigger than epsilon */
	for (i = 0; i < n; i++){
		if(fabs(vector_a[i] - vector_b[i]) > EPSILON){
			return -1;
		}
	}
	return 0;
}
