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

double abs_sum_of_double_vector(double* vec, int length){
	int i = 0;
	double sum = 0;

	for (i = 0; i < length; i++){
		sum = sum + fabs(vec[i]);
	}
	return sum;
}

double sum_of_integer_vector(int* vec, int length){
	int i = 0;
	int sum = 0;

	for (i = 0; i < length; i++){
		sum = sum + vec[i];
	}
	return sum;
}

double dot_product_auxiliary_sum(Network* N, double* x, Node* g, int n_g, int indicator){
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
		if (indicator == 1){
			mult = ((double)deg_vector[g_index] * (double)x[x_index]);
		}
		else{
			mult = (double)deg_vector[g_index];
		}
		result = result + mult;
		head = head->next;
		x_index += 1;
		cnt += 1;
	}
	return result;
}

void Bhat_multiplication(Network* N, double* x, double* result, Node* g, int n_g, double* row_sums,
		double* time_spent_in_spmat_mult, double* time_spent_in_first_sum, double* time_spent_in_final_result){
	double first_sum = 0;
	int ki = 0;
	int i = 0;
	Node* g_head = g;
	int g_index = 0;

	clock_t start, after_spmat_mult, after_first_sum, after_result_calc;
	/* Multiplying A*x, saving result in result vector */
	start = clock();
	spmat_mult(N->A, x, result, g);
	after_spmat_mult = clock();
	*time_spent_in_spmat_mult += (double)(after_spmat_mult - start);

	/* Calculating needed sums for rest of the multiplication */
	/* first_sum = sum(kj*xj) */
	first_sum = dot_product_auxiliary_sum(N,  x, g, n_g, 1);
	after_first_sum = clock();
	*time_spent_in_first_sum += (double)(after_first_sum - after_spmat_mult);

	/* Calculating final result vector */
	for (i = 0; i < n_g; i++){
		g_index = g_head->index;
		ki = N->deg_vector[g_index];
		result[i] = result[i] - (first_sum * ki) / N->M - row_sums[i] * x[i];
		g_head = g_head->next;
	}
	after_result_calc = clock();
	*time_spent_in_final_result += (double)(after_result_calc - after_first_sum);

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

void unit_vector_j(double* v, int n, int j){
	int i = 0;

	for (i = 0; i < n; i++){
		if (i == j){
			v[i] = 1;
		}
		else {
			v[i] = 0;
		}
	}
}

double Bhat_norm(Network* N, Node* g, int n_g, double* row_sums){
	double norm = 0;
	int j = 0;
	double* ej = NULL;
	double* B_col = NULL;
	double col_sum = 0;
	double a = 0;
	double b = 0;
	double c = 0;

	ej = (double*)allocate(n_g * sizeof(double));
	B_col = (double*)allocate(n_g * sizeof(double));

	/* Calculating all column absolute sums of B_hat[g], finding max sum */
	for (j = 0; j < n_g; j++){
		unit_vector_j(ej, n_g, j);
		Bhat_multiplication(N, ej, B_col, g, n_g, row_sums, &a, &b, &c);
		col_sum = abs_sum_of_double_vector(B_col, n_g);
		if ((j == 0) || (col_sum > norm)){
			norm = col_sum;
		}
	}
	free(ej);
	free(B_col);
	return norm;
}

double Bhat_largest_eigenvalue(Network* N, double norm, double* eigen_vector, int n_g, Node* g, double* row_sums){
	double numerator = 0;
	double denominator = 0;
	double* mul = NULL;
	double lambda = 0;
	double eigen_value = 0;
	double a = 0;
	double b = 0;
	double c = 0;

	/* Calculating numerator */
	mul = (double*)allocate(n_g * sizeof(double));
	Bhat_multiplication(N, eigen_vector, mul, g, n_g, row_sums, &a, &b, &c);
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
