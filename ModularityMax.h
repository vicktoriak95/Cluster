/*
 * ModularityMax.h
 *
 * Maximizes division modularity Q and updates the division vector s accordingly.
 *
 */

#ifndef MODULARITYMAX_H_
#define MODULARITYMAX_H_
#include "Network.h"
#include "NodeUtils.h"

/* INT_MAX == 2 ** 31 - 1 */
#define INT_MAX_POWER_OF_2 30

/* Receives network N, s which represents division, and g the group we dividing.
 * Improving modularity of division, updating s to match best division found.
 * According to algorithm 4 */
void modularity_maximization(Network* N, spmat* A, double* s, Node* g, int n_g, double* row_sums);

/* Calculating difference in Q after moving vertex.
 * Calculation is based on formulas that were derived from (s^t * B * s - d^t * B * s) */
double calc_Q_diff(double* s, int k, int real_k, Network* N, double A_sum, double aux_sum);

/* Updates A_sums according to moving vertex k between groups */
void update_A_sums(double* A_sums, int k, spmat* A, double* s);

/* Calculates auxiliary sum for modularity maximization */
double calc_sk_aux_sum(Network* N, double* s, Node* g, int n_g);

/* Moves all the remaining vertices (following max_improve_index) back to their original group
 * i.e. for all k > max_improve_index, s[k] = (-1) * s[k] */
void flip_s(double* s, int* indices, int n_g, int max_improve_index);

/* Searching for the best (provides best improvement in modularity) node to move among unmoved.
 * Updates max_diff, max_diff_index, real_max_diff_index, and Q_max accordingly */
void find_best_vertex_to_move(Network* N, Node* g, double* s, int n_g, int* unmoved,
		double base_aux_sum, double* A_sums, double* max_diff, int* max_diff_index, int* real_max_diff_index, double* Q_max, double Q_0);

/* Moving n_g vertices and finding best improve over all divisions checked. */
void find_best_improve(Network* N, spmat* A, Node* g, int n_g, double* A_sums,
		double* s, int* unmoved, double* Q_0, int* indices, double* max_improve, int* max_improve_index);

/* Calculates Q by formula: s^t * B_hat[g] * s */
double calc_Qk(Network* N, spmat* A, double* s, Node* g, int n_g, double* row_sums);

#endif /* MODULARITYMAX_H_ */
