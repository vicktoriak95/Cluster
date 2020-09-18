/*
 * Network.h
 *
 *	Defines Strcut Network which contains all information about given network:
 *	adjacency matrix, degree vector, sum of degrees and number of vertices.
 *
 *	Functions working with networks: allocating and freeing network,
 *	creation of network from file and from args and writing clusters to output.
 *
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "SparseMatrix.h"
#include "LibFuncsHandler.h"

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

/* Writes clusters in O to output file */
void write_clusters_to_output(Group* O, FILE* f, int n);

/* Free all allocated space of a network */
void free_network(Network* net);

/* Creates network from args */
Network* network_from_args(spmat* matrix, int* deg_vector, int n, int M);

#endif /* NETWORK_H_ */
