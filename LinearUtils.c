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
#include <assert.h>
#include <math.h>
#include "LibFuncsHandler.h"


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
double abs_sum_of_double_vector(double* vec, int length){
	int i;
	double sum = 0;
	for (i = 0; i < length; i++){
		sum = sum + fabs(vec[i]);
	}
	return sum;
}

/* Sum all entries of a given integer vector*/
double sum_of_integer_vector(int* vec, int length){
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
	Node* head = g;
	double mult;
	int* deg_vector = N->deg_vector;
	int M = N->M;
	int x_index = 0;

	/* Iterating over g indices and calculating sum */
	while(head != NULL){
		g_index = head->index;
		if (indicator == 1){
			mult = ((double)deg_vector[g_index] * (double)x[x_index]) / M;
		}
		else{
			mult = (double)deg_vector[g_index] / M;
		}
		result = result + mult;
		head = head->next;
		x_index += 1;
	}
	return result;
}

/* Multiplication of B\hat[g] with vector */
/* TODO: erase prints */
void Bhat_multiplication(Network* N, double* x, double* result, Node* g, int n_g){
	double first_sum = 0;
	double second_sum = 0;
	int A_row_sum = 0;
	int ki = 0;
	int i;
	Node* g_head = g;
	int g_index = 0;

	/* Multiplying A*x, saving result in result vector */
	spmat_mult(N->A, x, result, g, n_g);
	/* printf("A @ s: \n");
	print_vector(result, n_g); */
	/* Calculating needed sums for rest of the multiplication */
	first_sum = dot_product_auxiliary_sum(N,  x, g, n_g, 1);
	/* printf("sum1= %f \n", first_sum);*/
	second_sum = dot_product_auxiliary_sum(N, x, g, n_g, 2);
	/* printf("sum2= %f \n", second_sum); */
	/* Calculating final result vector */
	for(i=0; i<n_g; i++){
		A_row_sum = spmat_row_sum(N->A, i, g, n_g);
		g_index = g_head->index;
		ki = N->deg_vector[g_index];
		result[i] = result[i] - (first_sum * ki) + (second_sum * ki * x[i]) - (A_row_sum * x[i]);
		g_head = g_head->next;
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

/* Updates v such that entry j is 1 and every other entry is 0*/
void unit_vector_j(double* v, int n, int j){
	int i;

	for(i=0; i<n; i++){
		if(i == j){
			v[i] = 1;
		}
		else{
			v[i] = 0;
		}
	}
}

/* Calculating dot product of B\hat[g] */
double Bhat_norm(Network* N, Node* g, int n_g){
	double norm = -INFINITY;
	int j;
	double* ej;
	double* B_col;
	double col_sum;

	ej = (double*)allocate(n_g * sizeof(double));
	B_col = (double*)allocate(n_g * sizeof(double));

	for(j=0; j<n_g; j++){
		unit_vector_j(ej, n_g, j);
		Bhat_multiplication(N, ej, B_col, g, n_g);
		col_sum = abs_sum_of_double_vector(B_col, n_g);
		if(col_sum > norm){
			norm = col_sum;
		}
	}
	free(ej);
	free(B_col);
	return norm;
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
	/* TODO: add assert from libfunchanler */
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

void Bhat_tests(){
	Node* g;
	int n = 4;
	int n_g = 3;
	int i;
	spmat* A;
	Network* net;
	int deg_vector[4] = {1, 3, 2, 2};
	int matrix[4][4] = {{0, 1, 0, 0}, {1, 0, 1, 1}, {0, 1, 0, 1}, {0, 1, 1, 0}};
	int g_vector[3] = {0, 2, 3};
	double s[3] = {1, -1, 1};
	int M = 8;
	double* result;
	double norm = 0;


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
	result = (double*)malloc(n_g*sizeof(double));
	assert(result != NULL);

	/* ************* TESTING DOT PRODUCT ******************* */

	Bhat_multiplication(net, (double*)s, (double*)result, g, n_g);
	printf("dot product is: ");
	print_vector(result, n_g);


	/* ************* TESTING NORM ******************* */

	norm = Bhat_norm(net, g, n_g);
	printf("norm: %f \n", norm);

	/* ********* TESTING SHIFTED DOT PRODUCT *************** */
	Bhat_shift(result, s, norm, n_g);
	printf("shifted dot product: ");
	print_vector(result, n_g);

	free(result);
}

/*
int main(int argc, char* argv[]){
	Bhat_tests();

}
*/

