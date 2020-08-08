/*
 * LinearUtils.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#ifndef LINEARUTILS_H_
#define LINEARUTILS_H_

/* Dot product between two vectors */
double dot_product(double* vector1, double* vector2, int vector_size);

/* Prints vector */
void print_vector(double* vector, int vector_size);

/* Sum all entries of a given integer vector*/
int sum_of_vector(int* vec, int length);

/* Multiplication of B\hat[g] with vector */
/* Not shifting */
double* Bhat_multiplication(Network* N, double* x, Node* g, int n_g);

/* Adding vector2 to vector1 */
void vectors_sum(double* vector1, double* vector2, int length);

/* multiplying all entries of a given vector by a scalar */
void mult_vector_by_scalar(double* vector, double scalar, int length);

/* Shifting dot product B\hat[g] by B\hat[g] norm */
void Bhat_shift(double* dot_product, double* x, double norm, int length);

/* Calculating dot product of B\hat[g] */
double Bhat_norm(Network* N, int n_g);

/* Calculates the eigenvalue corresponding to largest eigen_vector */
/* Shifts result by norm */
double Bhat_largest_eigenvalue(struct Network* N, double norm, double* eigen_vector, int n_g);

#endif /* LINEARUTILS_H_ */
