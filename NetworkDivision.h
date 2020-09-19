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
void devide_into_two(Network* N, Group* group, double* s, double B_norm);

/* Calculating vector s matching to given eigen-vector */
void calculate_s(double* eigen_vector, double* s, int n_g);

/* Dividing group into two groups */
void divide_group(Network* N, Group* old_group, double* s, Group** new_group1, Group** new_group2);

/* Pushing groups received from division into O and P */
void groups_into_O_P(Group** O, Group** P, Group** group1, Group** group2);

/* Calculating new groups length */
int calc_group_length(double* s, int n);

/* Reads and prints output file containing division into clusters */
void print_output_file(FILE* output_file);

#endif /* NETWORKDIVISION_H_ */
