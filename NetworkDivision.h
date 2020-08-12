/*
 * NetworkDevision.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#ifndef NETWORKDIVISION_H_
#define NETWORKDIVISION_H_
#include <stdio.h>
#include "NodeUtils.h"
#include "Network.h"

/* Main - dividing network into clusters */
int main(FILE* input, FILE* output);

/* Writes clusters in O to output file */
void write_clusters_to_output(Group* O, FILE* f);

/* Divides g into two groups */
/* Returns vector s which represents the division of g */
int* devide_into_two(Network* N, Node* g);

/* Calculating vector s matching to given eigen-vector*/
void calculate_s(double* eigen_vector, double* s, int n);

/* Dividing g according to s */
/* Editing group g into group g1 ,and returns group g2 */
Node* divide_group(Node* g1, int* s, int n);

/* Improving modularity */
void modularity_maximization(int* s, Node* g, int n_g);

#endif /* NETWORKDIVISION_H_ */
