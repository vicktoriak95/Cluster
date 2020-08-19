/*
 * NetworkDevision.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#ifndef NETWORKDIVISION_H_
#define NETWORKDIVISION_H_

#include <stdio.h>
#include "Network.h"

void divide_net_to_clusters(FILE* input, FILE* output);

/* Fills s with 1.0 - meaning all vertices are in the same group */
void indivisable(double* s, int n_g);

/* Divides g into two groups */
/* Returns vector s which represents the division of g */
void devide_into_two(Network* N, Node* g, double* s, int n_g);

/* Calculating vector s matching to given eigen-vector*/
void calculate_s(double* eigen_vector, double* s, int n_g);

/* Dividing g according to s */
/* Editing group g into group g1 ,and returns group g2 */
Node* divide_group(Node** g1_p, double* s, int n_g);

/* Calculates s^t * b_hat[g] * s */
double calc_Qk(Network* N, double* s, Node* g, int n_g);

/* Improving modularity */
void modularity_maximization(Network* N, double* s, Node* g, int n_g);

void print_output_file(FILE* output_file);

#endif /* NETWORKDIVISION_H_ */
