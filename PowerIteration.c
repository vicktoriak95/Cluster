/*
 * PowerIteration.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PowerIteration.h"
#include "LibFuncsHandler.h"
#include "LinearUtils.h"

double* power_iteration(Network* N, double norm, Node* g, int n_g, double* row_sums){
	double* temp = NULL;
	double* b_prev = NULL;
	double* b_next = NULL;
	double vector_norm = 0;
	int cnt = 0;

	/* Initiating b_prev to be random b_0, b_next to be random too */
	b_prev = (double*)allocate(n_g * sizeof(double));
	create_random_vector(b_prev, n_g);
	b_next =(double*)allocate(n_g * sizeof(double));
	create_random_vector(b_next, n_g);

	/* While vectors diff bigger than epsilon */
	while(close_vectors(b_prev, b_next, n_g) != 0){
		infinite_loop_detection(cnt, MAX_POWER_ITERATIONS);

		Bhat_multiplication(N, b_prev, b_next, g, n_g, row_sums);

		Bhat_shift(b_next, b_prev, norm, n_g);

		/* Normalizing over norm */
		vector_norm = sqrt(dot_product(b_next, b_next, n_g));
		assert_not_zero(vector_norm);
		mult_vector_by_scalar(b_next, 1 / vector_norm, n_g);

		temp = b_prev;
		b_prev = b_next;
		b_next = temp;
		cnt += 1;
	}
	free(b_next);

	return b_prev;
}

void create_random_vector(double * vector, int vector_size){
	int i = 0;
	for (i = 0; i < vector_size; i++){
		vector[i] = (double)rand();
	}
}

int close_vectors(double* vector_a, double* vector_b, int n){
	int i = 0;
	/* Iterating over vectors, checking if diff of every entry is bigger than epsilon */
	for (i = 0; i < n; i++){
		if (fabs(vector_a[i] - vector_b[i]) >= EPSILON){
			return -1;
		}
	}
	return 0;
}
