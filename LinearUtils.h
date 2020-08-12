/*
 * LinearUtils.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#include "Network.h"

#ifndef LINEARUTILS_H_
#define LINEARUTILS_H_

/* Dot product between two vectors */
double dot_product(double* vector1, double* vector2, int vector_size);

/* Prints vector */
void print_vector(double* vector, int vector_size);

/* Sum all entries of a given double vector*/
double sum_of_double_vector(double* vec, int length);

/* Sum all entries of a given integer vector*/
double sum_of_integer_vector(int* vec, int length);

/* Multiplication of B\hat[g] with vector */
/* Not shifting */
void Bhat_multiplication(Network* N, double* x, double* result, Node* g, int n_g);

/* Calculating auxiliary sum for B\hat[g] dot product */
double dot_product_auxiliary_sum(double* deg_vector, int M, double* x, Node* g, int n_g, int indicator);

/* Adding vector2 to vector1 */
void vectors_sum(double* vector1, double* vector2, int length);

/* multiplying all entries of a given vector by a scalar */
void mult_vector_by_scalar(double* vector, double scalar, int length);

/* Shifting dot product B\hat[g] by B\hat[g] norm */
void Bhat_shift(double* dot_product, double* x, double norm, int length);

/* Calculating dot product of B\hat[g] */
double Bhat_norm(Network* N, Node* g, int n_g);

/* Calculates the eigenvalue corresponding to largest eigen_vector */
/* Shifts result by norm */
double Bhat_largest_eigenvalue(Network* N, double norm, double* eigen_vector, int n_g);

/* Updates v such that entry j is 1 and every other entry is 0*/
void unit_vector_j(double* v, int n, int j);

#endif /* LINEARUTILS_H_ */
