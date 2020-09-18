/*
 * PowerIteration.h
 *
 * Functions implementing power iteration algorithm which calculates eigen vector of matrix A.
 *
 */

#ifndef POWERITERATION_H_
#define POWERITERATION_H_
#define MAX_POWER_ITERATIONS 100000

#include <time.h>

#include "Network.h"

/* Finds eigen-vector with biggest eigen-value of matrix A */
double* power_iteration(Network* N, double norm, Node* g, int n_g, double* row_sums);

/* Receives allocated vector "vector" and updates it with random doubles */
void create_random_vector(double * vector, int vector_size);

/* Checks if difference between vector_a and vector_b is bigger than epsilon.
 * Returns -1 if difference is bigger than epsilon, 0 otherwise. */
int close_vectors(double* vector_a, double* vector_b, int n);

#endif /* POWERITERATION_H_ */
