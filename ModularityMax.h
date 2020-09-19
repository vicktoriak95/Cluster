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
void modularity_maximization(Network* N, spmat* A, double* s, Node* g, int n_g, double* row_sums);

/* Calculating difference in Q after moving vertex.
 * Calculation is based on formulas that were derived from (s^t * B * s - d^t * B * s)
 * where d is vector s after moving one vertex. */
double calc_Q_diff(double* s, int k, int real_k, Network* N, double A_sum, double aux_sum);

/* TODO */
void update_A_sums_new(double* A_sums, int k, int real_k, spmat* A, double* s);

/* TODO */
double calc_sk_aux_sum(Network* N, double* s, Node* g, int n_g);

/* Sums row values of sub-matrix A[g], according to g values,
 *  multiplied by values of vector d. */
double A_single_row_sum_by_vec(spmat* A, int mat_row_index, Node* g, double* vector);

/*TODO*/
void A_row_sums_by_vec(Node* g, Network* N, double* A_row_sums, int n_g, double* vector);

/*TODO*/
void flip_s(double* s, int* indices, int n_g, int max_improve_index);

/*TODO*/
void find_best_vertex_to_move(Network* N, Node* g, double* s, int n_g, int* unmoved,
		double base_aux_sum, double* A_sums, double* max_diff, int* max_diff_index, int* real_max_diff_index, double* Q_max, double Q_0);

/*TODO*/
void find_best_improve(Network* N, spmat* A, Node* g, int n_g, double* A_sums,
		double* s, int* unmoved, double* Q_0, int* indices, double* max_improve, int* max_improve_index);

/* Calculates Q by formula: s^t * B_hat[g] * s */
double calc_Qk(Network* N, spmat* A, double* s, Node* g, int n_g, double* row_sums);

#endif /* MODULARITYMAX_H_ */
