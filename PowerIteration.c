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
#include "Network.h"
#include "LibFuncsHandler.h"
#include "SparseMatrix.h"
# include "LinearUtils.h"

/* Finds eigen-vector with biggest eigen-value */
double* power_iteration(Network* N, double norm, Node* g, int n_g){
	double b_prev_const[3] = {0.36283495, 0.56467796, 0.66505637};
	double b_next_const[3] = {0.11767303, 0.5011049, 0.89936061};
	double* temp;
	int i;
	double* b_prev = b_prev_const;
	double* b_next = b_next_const;


	/* Initiating b_prev to be random b_0 */
	/*b_prev = (double*)allocate(n_g * sizeof(double));*/
	/*create_random_vector(b_prev, n_g);*/
	/*b_next =(double*)allocate(n_g * sizeof(double));*/

	/* TODO: How to assure loop is not infinite */
	/* While vectors diff bigger than epsilon */
	while(close_vectors(b_prev, b_next, n_g) != 0){
		/* Dot product between B\hat[g]_shifted and b_k */
		/* TODO: add result in multiplication */

		/* Multiplying B\hat by b_prev and saving result in b_next */
		Bhat_multiplication(N, b_prev, b_next, g, n_g);
		/* Shifting b_next by b_prev and norm*/
		Bhat_shift(b_next, b_prev, norm, n_g);
		printf("before norm vector is: \n");
		print_vector(b_next, n_g);

		/*
		spmat_mult(N->A, b_prev, b_next, g, n_g);*/

		norm = sqrt(dot_product(b_next, b_next, n_g));
		for(i = 0; i < n_g; i++){
			b_next[i] /= norm;
		}
		printf("after norm vector is: \n");
		/*print_vector(b_next, n_g);*/

		temp = b_prev;
		b_prev = b_next;
		b_next = temp;
	}
	free(b_next);
	return b_prev;
}

/* Creates random vector */
void create_random_vector(double * vector, int vector_size){
	int i;
	for(i=0; i<vector_size; i++){
		vector[i] = (double)rand();
	}
}

/* Checks if difference between two vectors is bigger than epsilon */
/* returns -1 if difference is bigger than epsilon, 0 otherwise. */
int close_vectors(double* vector_a, double* vector_b, int n){
	int i;
	/* Iterating over vectors, checking if diff of every entry is bigger than epsilon */
	for (i = 0; i < n; i++){
		if(fabs(vector_a[i] - vector_b[i]) > epsilon){
			return -1;
		}
	}
	return 0;
}

void test_power_iteration(){
	Node* g;
	int n = 4;
	int n_g = 3;
	int i;
	spmat* A;
	double* eigen_vector;
	Network* net;
	int deg_vector[4] = {1, 3, 2, 2};
	int matrix[4][4] = {{0, 1, 0, 0}, {1, 0, 1, 1}, {0, 1, 0, 1}, {0, 1, 1, 0}};
	int g_vector[3] = {0, 2, 3};
	/*int s[3] = {1, -1, 1};*/
	double norm = 0;
	int M = 8;


	g = node_list_from_vector(g_vector, n_g);

	A = spmat_allocate(n);
	for(i = 0; i < n; i++){
		spmat_add_row_from_vector(A, matrix[i], i);
	}
	printf("created A \n");

	/*
	A = spmat_from_matrix((int**)(&matrix), n);
	*/
	net = network_from_args(A, deg_vector, 4, M);
	norm = Bhat_norm(net, g, n_g);
	eigen_vector = power_iteration(net, norm, g, n_g);
	print_vector(eigen_vector, n_g);

	/* TODO: Free things */

}


int main(int argc, char* argv[]){
	test_power_iteration();

}


