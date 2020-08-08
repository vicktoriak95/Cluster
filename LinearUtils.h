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

/* Multiplication of B\hat[g] with vector */
/* Shifting is done separately */
double* Bhat_multiplication(Network* N, double* x);

/* Shifting dot product B\hat[g] by B\hat[g] norm */
void Bhat_shift(double* dot_product, double norm);

/* Calculating dot product of B\hat[g] */
double Bhat_norm(Network* N);

/* Calculates the eigenvalue corresponding to largest eigen_vector */
/* Shifts result by norm */
double Bhat_largest_eigenvalue(Network* N, double norm, double* eigen_vector);

#endif /* LINEARUTILS_H_ */
