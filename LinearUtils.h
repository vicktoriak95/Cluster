/*
 * LinearUtils.h
 *
 * Implementation of linear methods on vectors and matrices.
 * Classic linear methods such as dot product, sum of vector, etc.
 * Methods specific to division: B_hat multiplication by vector methods: Bhat_multiplication and Bhat_shift, Bhat_norm, etc.
 *
 */

#ifndef LINEARUTILS_H_
#define LINEARUTILS_H_

#include <time.h>
#include "Network.h"

/* Dot product between two vectors */
double dot_product(double* vector1, double* vector2, int vector_size);

/* Prints vector */
void print_vector(double* vector, int vector_size);

/* Sum all entries of a given double vector */
double abs_sum_of_double_vector(double* vec, int length);

/* Sum all entries of a given integer vector */
double sum_of_integer_vector(int* vec, int length);

/* Calculating auxiliary sum for B\hat[g] multiplication */
double dot_product_auxiliary_sum(Network* N, double* x, Node* g, int n_g, int indicator);

/* Multiplication of B\hat[g] with vector x.
 * Receives Network N, x the vector we multiplying and result vector,
 * writes multiplication result to result vector.
 * Multiplication using only adjacency matrix A, and degree vector,
 * calculation is based on formulas that were derived from the B_hat[g]*x dot product */
void Bhat_multiplication(Network* N, double* x, double* result, Node* g, int n_g, double* row_sums);

/* Multiplying all entries of a given vector by a scalar */
void mult_vector_by_scalar(double* vector, double scalar, int length);

/* Multiplication of B\hat[g]_shifted with vector x.
 * Receives dot_product - result of multiplication of B_hat[g] by vector x.
 * Calculating multiplication result and saves it in dot_product. */
void Bhat_shift(double* dot_product, double* x, double norm, int length);

/* Receives allocated vector v.
 * Updates v such that entry j is 1 and every other entry is 0. */
void unit_vector_j(double* v, int n, int j);

/* Calculating L1 norm of B\hat[g].
 * Calculation is based on formulas that were derived norm definition.
 */
double Bhat_norm(Network* N, Node* g, int n_g);

/* Calculates the eigenvalue corresponding to largest eigen_vector.
 * Shifts result by norm. */
double Bhat_largest_eigenvalue(Network* N, double norm, double* eigen_vector, int n_g, Node* g, double* row_sums);

/* Sums B[g] rows, i.e returns f_i^g = sum(A[g]_i,j - k_i*k_j/M) */
double B_row_sum(spmat* A, int mat_row_index, Node* g, Network* N, int abs);

/* TODO */
void B_row_sums(Node* g, Network* N, double* row_sums, int n_g);

#endif /* LINEARUTILS_H_ */
