/*
 * Network.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "SparseMatrix.h"

typedef struct _network{

	/* Adjacency matrix */
	spmat* A;

	/* Degrees vector */
	int* deg_vector;

	/* Sum of degrees */
	int M;

	/* Num of vertices in the network */
	int n;

} Network;

/* Allocating a new network with n vertices*/
Network* allocate_network(int n);

/* Creating new network from input file */
Network* create_network(FILE* input);

/* Free all allocated space of a network */
void* free_network(Network*);

#endif /* NETWORK_H_ */
