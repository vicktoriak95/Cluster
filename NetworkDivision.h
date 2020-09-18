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
#include <time.h>

/* Main algorithm, receives input and output file,
 * reads network from input file, calculates clusters and writes them to output file. */
void divide_net_to_clusters(FILE* input, FILE* output);

/* Fills s with 1.0 - meaning all vertices are in the same group */
void indivisable(double* s, int n_g);

/* Divides g into two groups by updating entries of vector s which represents the division of g */
void devide_into_two(Network* N, Node* g, double* s, int n_g, double B_norm, double* row_sums);

/* Calculating vector s matching to given eigen-vector */
void calculate_s(double* eigen_vector, double* s, int n_g);

/* Dividing g according to s.
 * Editing group g into group g1 ,and returns group g2 */
Node* divide_group(Node** g1_p, double* s, int n_g);

/* Reads and prints output file containing division into clusters */
void print_output_file(FILE* output_file);

#endif /* NETWORKDIVISION_H_ */
