/*
 * ModularityMax.h
 *
 * Maximizes division modularity and updates s accordingly.
 *
 */

#ifndef MODULARITYMAX_H_
#define MODULARITYMAX_H_
#include "Network.h"
#include "NodeUtils.h"

/* INT_MAX == 2 ** 31 - 1 */
#define INT_MAX_POWER_OF_2 30

/* Receives network N, s which represents division, and g the group we dividing.
 * Improving modularity of division, updating s to match best division found */
void modularity_maximization(Network* N, double* s, Node* g, int n_g, double* row_sums);

/* Calculating difference in Q after moving vertex.
 * Calculation is based on formulas that were derived from (s^t * B * s - d^t * B * s)
 * where d is vector s after moving one vertex. */
double calc_Q_diff(double* s, int k, int real_k, Network* N, double A_sum, double aux_sum);

/* TODO */
void update_A_sums(double* A_sums, int k, int real_k, Network* N, double* s, Node* g);

/* TODO */
double aux_sum_score(Network* N, double* s, Node* g, int n_g);

#endif /* MODULARITYMAX_H_ */
