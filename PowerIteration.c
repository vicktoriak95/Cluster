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
	/*
	clock_t after_bhat_mult, after_bhat_shift, after_vecotr_norm;
	*/
	clock_t start, after_random_vectors, finish;
	clock_t before_bhat_mult, after_bhat_mult;
	clock_t before_bhat_shift, after_bhat_shift;
	clock_t before_bhat_remain, after_bhat_remain;
	double time_spent_in_bhat_mult = 0;
	double time_spent_in_bhat_shift = 0;
	double time_spent_in_bhat_remain = 0;

	printf("### Entered into power iteration ###\n");
	start = clock();
	/* Initiating b_prev to be random b_0, b_next to be random too */
	b_prev = (double*)allocate(n_g * sizeof(double));
	create_random_vector(b_prev, n_g);
	b_next =(double*)allocate(n_g * sizeof(double));
	create_random_vector(b_next, n_g);
	after_random_vectors = clock();
	printf("Time up to after_random_vectors: %f seconds\n", ((double)(after_random_vectors-start) / CLOCKS_PER_SEC));

	/* While vectors diff bigger than epsilon */
	while(close_vectors(b_prev, b_next, n_g) != 0){
		infinite_loop_detection(cnt, MAX_POWER_ITERATIONS);

		before_bhat_mult = clock();
		/* Calculating dot product between B\hat[g]_shifted and b_k:
		 * Multiplying B\hat by b_prev and saving result in b_next */
		Bhat_multiplication(N, b_prev, b_next, g, n_g, row_sums);
		/*
		after_bhat_mult = clock();
		printf("Time up to after_bhat_mult: %f seconds\n", ((double)(after_bhat_mult-start) / CLOCKS_PER_SEC));
		*/
		after_bhat_mult = clock();
		time_spent_in_bhat_mult += (double)(after_bhat_mult-before_bhat_mult);


		before_bhat_shift = clock();
		/* Shifting b_next by b_prev and norm*/
		Bhat_shift(b_next, b_prev, norm, n_g);
		/*
		after_bhat_shift = clock();
		printf("Time up to after_bhat_shift: %f seconds\n", ((double)(after_bhat_shift-start) / CLOCKS_PER_SEC));
		*/
		after_bhat_shift = clock();
		time_spent_in_bhat_shift += (double)(after_bhat_shift-before_bhat_shift);

		before_bhat_remain = clock();
		/* Normalizing over norm */
		vector_norm = sqrt(dot_product(b_next, b_next, n_g));
		assert_not_zero(vector_norm);
		mult_vector_by_scalar(b_next, 1 / vector_norm, n_g);
		/*
		after_vecotr_norm = clock();
		printf("Time up to after_vecotr_norm: %f seconds\n", ((double)(after_vecotr_norm-start) / CLOCKS_PER_SEC));
		*/

		temp = b_prev;
		b_prev = b_next;
		b_next = temp;
		cnt += 1;
		after_bhat_remain = clock();
		time_spent_in_bhat_remain += (double)(after_bhat_remain-before_bhat_remain);
	}
	free(b_next);
	finish = clock();
	printf("Time up to finish: %f seconds\n", ((double)(finish-start) / CLOCKS_PER_SEC));
	printf("time_spent_in_bhat_mult: %f seconds\n", (time_spent_in_bhat_mult / CLOCKS_PER_SEC));
	printf("time_spent_in_bhat_shift: %f seconds\n", (time_spent_in_bhat_shift / CLOCKS_PER_SEC));
	printf("time_spent_in_bhat_remain: %f seconds\n", (time_spent_in_bhat_remain / CLOCKS_PER_SEC));
	printf("### Exited power iteration ###\n");
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
