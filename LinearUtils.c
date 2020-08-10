/*
 * LinearUtils.c
 *
 *  Created on: Aug 8, 2020
 *      Author: adi
 */


#include <stdlib.h>
#include <stdio.h>
#include "NodeUtils.h"
#include "Network.h"
#include "SparseMatrix.h"


/* Dot product between two vectors */
double dot_product(double* vector1, double* vector2, int vector_size){
	double sum = 0;
	double product;
	int i;
	for (i = 0; i < vector_size; i++){
		product = vector1[i] * vector2[i];
		sum = sum + product;
	}
	return sum;
}

/* Prints vector */
void print_vector(double* vector, int vector_size){
	int i;

	for (i = 0; i < vector_size; i++){
		printf("%f ", vector[i]);
	}
	printf("\n");

}

/* Sums all entries of a given integer vector*/
int sum_of_vector(int* vec, int length){
	int i;
	int sum = 0;
	for (i = 0; i < length; i++){
		sum = sum + vec[i];
	}
	return sum;
}

/* Calculating auxiliary sum for B\hat[g] dot product */
double dot_product_auxiliary_sum(Network* N, double* x, Node* g, int n_g, int indicator){
	double result = 0;
	int g_index;
	Node* head = NULL;
	int mult;
	int* deg_vector = N->deg_vector;
	int M = N->M;


	/* Iterating over g indices and calculating sum */
	while(head != NULL){
		g_index = head->index;
		if (indicator == 1){
			mult = (deg_vector[g_index] * x[g_index]) / M;
		}
		else{
			mult = deg_vector[g_index] / M;
		}
		result = result + mult;
	}
	return result;
}

/* Multiplication of B\hat[g] with vector */
void Bhat_multiplication(Network* N, double* x, double* result, Node* g, int n_g){
	double first_sum = 0;
	double second_sum = 0;
	int A_row_sum = 0;
	int ki = 0;
	int i;

	/* Multiplying A*x, saving result in result vector */
	spmat_mult(N->A, x, result, g, n_g);
	/* Calculating needed sums for rest of the multiplication */
	first_sum = dot_product_auxiliary_sum(N,  x, g, n_g, 1);
	second_sum = dot_product_auxiliary_sum(N, x, g, n_g, 2);
	/* Calculating final result vector */
	for(i=0; i<n_g; i++){
		/* TODO: MAKE SURE THE RIGHT INDEX */
		A_row_sum = spmat_row_sum(N->A, i, g, n_g);
		ki = N->deg_vector[i];
		/* TODO: MAKE SURE IS FINE SUBSTRACTING ROW_SUM = INT */
		result[i] = result[i] - (first_sum * ki) + (second_sum * ki) - A_row_sum;
	}
}


/* Adding vector2 to vector1 */
void vectors_sum(double* vector1, double* vector2, int length){
	int i;
	for (i = 0; i < length; i++){
		vector1[i] = vector1[i] + vector2[i];
	}
}

/* Multiplying all entries of a given vector by a scalar */
void mult_vector_by_scalar(double* vector, double scalar, int length){
	int i;
	for (i = 0; i < length; i++){
		vector[i] = vector[i] * scalar;
	}
}

/* Shifting dot product B\hat[g] by B\hat[g] norm */
void Bhat_shift(double* dot_product, double* x, double norm, int length){
	int i;
	double prod;

	for (i = 0; i < length; i++){
		prod = x[i] * norm;
		dot_product[i] = dot_product[i] + prod;
	}
}

/* Calculating dot product of B\hat[g] */
double Bhat_norm(Network* N, int n_g){
	/* TODO: This is crap*/
	double result = 0;
	return result;
}

/* Calculates the eigenvalue corresponding to largest eigen_vector */
/* Shifts result by norm */
double Bhat_largest_eigenvalue(Network* N, double norm, double* eigen_vector, int n_g, Node* g){
	double numerator;
	double denominator;
	double* mul;
	double lambda;
	double eigen_value;

	/* Calculating numerator */
	mul = (double*)malloc(n_g*sizeof(double));
	Bhat_multiplication(N, eigen_vector, mul, g, n_g);
	/* Shifting mul */
	Bhat_shift(mul, eigen_vector, norm, n_g);
	numerator = dot_product(eigen_vector, mul, n_g);
	/* Calculating denominator */
	denominator = dot_product(eigen_vector, eigen_vector, n_g);
	/* Calculating lambda */
	lambda = numerator / denominator;
	eigen_value = lambda - norm;
	free(mul);

	return eigen_value;

}

