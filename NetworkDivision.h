/*
 * NetworkDevision.h
 *
 * Functions used to divide network into clusters, such as divide_net_to_clusters
 * which uses method as divide into two, modularity maximization etc.
 *
 */

#ifndef NETWORKDIVISION_H_
#define NETWORKDIVISION_H_

#include <stdio.h>
#include "Network.h"

/* INT_MAX == 2 ** 31 - 1 */
#define INT_MAX_POWER_OF_2 30

/* Main algorithm, receives input and output file,
 * reads network from input file, calculates clusters and writes them to output file. */
void divide_net_to_clusters(FILE* input, FILE* output);

/* Fills s with 1.0 - meaning all vertices are in the same group */
void indivisable(double* s, int n_g);

/* Divides g into two groups by updating entries of vector s which represents the division of g */
void devide_into_two(Network* N, Node* g, double* s, int n_g);

/* Calculating vector s matching to given eigen-vector */
void calculate_s(double* eigen_vector, double* s, int n_g);

/* Dividing g according to s.
 * Editing group g into group g1 ,and returns group g2 */
Node* divide_group(Node** g1_p, double* s, int n_g);

/* Calculates Q by formula: s^t * B_hat[g] * s */
double calc_Qk(Network* N, double* s, Node* g, int n_g);

/* Calcluting difference in Q after moving vertex.
 * Calculation is based on formulas that were derived from (s^t * B * s - d^t * B * s)
 * where d is vector s after moving one vertex. */
double calc_Q_diff(double* d, int i, Network* N, Node* g, int n_g);

/* Receives network N, s which represents division, and g the group we dividing.
 * Improving modularity of division, updating s to match best division found */
void modularity_maximization(Network* N, double* s, Node* g, int n_g);

/* Reads and prints output file containing division into clusters */
void print_output_file(FILE* output_file);

#endif /* NETWORKDIVISION_H_ */
