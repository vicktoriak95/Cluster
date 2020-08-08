/*
 * Network.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#ifndef NETWORK_H_
#define NETWORK_H_

typedef struct _network{

	/* Adjacency matrix */
	struct spmat* A;

	/* Degrees vector */
	int* deg_vector;

	/* Sum of degrees */
	int M;

	/* Num of vertices in the network */
	int n;

} Network;

/* Creating new network from input file */
Network* create_network(FILE* input);

#endif /* NETWORK_H_ */
