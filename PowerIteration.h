/*
 * PowerIteration.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#ifndef POWERITERATION_H_
#define POWERITERATION_H_
#define EPSILON 0.00001

#include "Network.h"

/* Finds eigen-vector with biggest eigen-value */
double* power_iteration(Network* N, double norm, Node* g, int n_g);

/* Creates random vector */
void create_random_vector(double * vector, int vector_size);

/* Checks if difference between two vectors is bigger than epsilon */
/* returns -1 if difference is bigger than epsilon, 0 otherwise. */
int close_vectors(double* vector_a, double* vector_b, int n);

#endif /* POWERITERATION_H_ */
