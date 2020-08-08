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

void print_vector(double* vector, int vector_size){
	int i;

	for (i = 0; i < vector_size; i++){
		printf("%f + , ", vector[i]);
	}
	printf("\n");

}

/* Sum all entries of a given integer vector*/
int sum_of_vector(int* vec, int length){
	int i;
	int sum = 0;
	for (i = 0; i < length; i++){
		sum = sum + vec[i];
	}
	return sum;
}

/* Multiplication of B\hat[g] with vector */
double* Bhat_multiplication(Network* N, double* x, Node* g, int n_g){
	/* TODO: This is crap*/
	double* result = NULL;
	return result;
}

/* Adding vector2 to vector1 */
void vectors_sum(double* vector1, double* vector2, int length){
	int i;
	for (i = 0; i < length; i++){
		vector1[i] = vector1[i] + vector2[i];
	}
}

/* multiplying all entries of a given vector by a scalar */
void mult_vector_by_scalar(double* vector, double scalar, int length){
	int i;
	for (i = 0; i < length; i++){
		vector[i] = vector[i]*scalar;
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


	mul = (double*)malloc(n_g*sizeof(double));
	mul = Bhat_multiplication(N, eigen_vector, g, n_g);
	/* shifting mul */
	Bhat_shift(mul, eigen_vector, norm, n_g);
	numerator = dot_product(eigen_vector, mul, n_g);
	denominator = dot_product(eigen_vector, eigen_vector, n_g);
	lambda = numerator / denominator;
	eigen_value = lambda - norm;

	return eigen_value;

}

