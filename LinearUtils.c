/*
 * LinearUtils.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "SparseMatrix.h"
#include "LibFuncsHandler.h"
#include "LinearUtils.h"
#include "Group.h"

double dot_product(double* vector1, double* vector2, int vector_size){
	double sum = 0;
	double product = 0;
	int i = 0;

	for (i = 0; i < vector_size; i++){
		product = vector1[i] * vector2[i];
		sum = sum + product;
	}
	return sum;
}

void print_vector(double* vector, int vector_size){
	int i = 0;

	for (i = 0; i < vector_size; i++){
		printf("%f ", vector[i]);
	}
	printf("\n");
}

void print_int_vector(int* vector, int vector_size){
	int i = 0;

	for (i = 0; i < vector_size; i++){
		printf("%d ", vector[i]);
	}
	printf("\n");
}

double sum_of_integer_vector(int* vec, int length){
	int i = 0;
	int sum = 0;

	for (i = 0; i < length; i++){
		sum = sum + vec[i];
	}
	return sum;
}

double dot_product_auxiliary_sum(Network* N, double* x, Node* g, int n_g){
	double result = 0;
	int g_index = 0;
	Node* head = g;
	double mult = 0;
	int* deg_vector = N->deg_vector;
	int x_index = 0;
	int cnt = 0;

	/* Iterating over g indices and calculating sum */
	while(head != NULL){
		infinite_loop_detection(cnt, n_g);
		g_index = head->index;
		mult = ((double)deg_vector[g_index] * (double)x[x_index]);
		result = result + mult;
		head = head->next;
		x_index += 1;
		cnt += 1;
	}
	return result;
}

void Bhat_multiplication(Network* N, spmat* A, double* x, double* result, Node* g, int n_g, double* row_sums){
	double aux_sum = 0;
	int ki = 0;
	int i = 0;
	Node* g_head = g;
	int g_index = 0;

	/* Multiplying A*x, saving result in result vector */
	spmat_mult(A, x, result);

	/* Calculating needed sums for rest of the multiplication
	 * aux_sum = sum(kj*xj) */
	aux_sum = dot_product_auxiliary_sum(N, x, g, n_g);

	/* Calculating final result vector */
	for (i = 0; i < n_g; i++){
		g_index = g_head->index;
		ki = N->deg_vector[g_index];
		result[i] = result[i] - (aux_sum * ki) / N->M - row_sums[i] * x[i];
		g_head = g_head->next;
	}

}

void mult_vector_by_scalar(double* vector, double scalar, int length){
	int i = 0;

	for (i = 0; i < length; i++){
		vector[i] = vector[i] * scalar;
	}
}

void Bhat_shift(double* dot_product, double* x, double norm, int length){
	int i = 0;
	double prod = 0;

	for (i = 0; i < length; i++){
		prod = x[i] * norm;
		dot_product[i] = dot_product[i] + prod;
	}
}

double Bhat_norm(Network* N, Group* group){
	Node* g_row_head = group->vertices;
	double row_abs_sum = 0;
	int real_mat_row_index = 0;
	int i = 0;
	double norm = -HUGE_VAL;

	/* Calculating sum for each row */
	for(i = 0; i < group->A_g->n; i++){
		/* Find row index in A*/
		real_mat_row_index = g_row_head->index;

		row_abs_sum = B_single_row_sum(group, N, i, real_mat_row_index, 1);
		if(row_abs_sum > norm){
			norm = row_abs_sum;
		}
		g_row_head = g_row_head->next;
	}
	return norm;
}

double Bhat_largest_eigenvalue(Network* N, spmat* A, double norm, double* eigen_vector, int n_g, Node* g, double* row_sums){
	double numerator = 0;
	double denominator = 0;
	double* mul = NULL;
	double lambda = 0;
	double eigen_value = 0;

	/* Calculating numerator */
	mul = (double*)allocate(n_g * sizeof(double));
	Bhat_multiplication(N, A, eigen_vector, mul, g, n_g, row_sums);
	/* Shifting mul */
	Bhat_shift(mul, eigen_vector, norm, n_g);
	numerator = dot_product(eigen_vector, mul, n_g);
	/* Calculating denominator */
	denominator = dot_product(eigen_vector, eigen_vector, n_g);
	/* Calculating lambda */
	assert_not_zero(denominator);
	lambda = numerator / denominator;
	eigen_value = lambda - norm;
	free(mul);

	return eigen_value;
}

double B_single_row_sum(Group* group, Network* N, int mat_row_index, int real_mat_row_index, int abs){
	int j = 0;
	Node_matrix* row_head = NULL;
	Node* vertices_head = group->vertices;
	double sum = 0;
	spmat* A = group->A_g;
	int* deg_vector = N->deg_vector;
	int M = N->M;
	double kikj_M = 0;
	int val = 0;

	row_head = ((Node_matrix** )A->private)[mat_row_index];
	for(j = 0; j < A->n; j++){
		kikj_M = ((double)deg_vector[vertices_head->index] * deg_vector[real_mat_row_index]) / M;
		/* If entry is not zero*/
		if((row_head != NULL) && (j == row_head->col_index)){
			val = 1;
		}
		else{
			val = 0;
		}
		if(abs == 0){
			sum += val - kikj_M;
		}
		else{
			sum += fabs(val - kikj_M);
		}
		vertices_head = vertices_head->next;
	}
	return sum;
}

void B_row_sums(Group* group, Network* N){
	Node* g_row_head = group->vertices;
	double* row_sums = group->row_sums;
	double row_sum = 0;
	int real_mat_row_index = 0;
	int i = 0;

	/* Calculating sum for each row */
	for(i=0; i<group->A_g->n; i++){
		/* Find row index in A*/
		real_mat_row_index = g_row_head->index;

		row_sum = B_single_row_sum(group, N, i, real_mat_row_index, 0);

		row_sums[i] = row_sum;
		g_row_head = g_row_head->next;
	}
}
