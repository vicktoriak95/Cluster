#include <stdlib.h>
#include <stdio.h>

#include "Network.h"
#include "SparseMatrix.h"
#include "LibFuncsHandler.h"

Network* allocate_network(int n){
	Network* net = NULL;
	spmat* A = NULL;
	int* deg_vector = NULL;

	/* allocate the network */
	net = (Network*)allocate(sizeof(Network));

	/* allocate the adjacency matrix */
	A = spmat_allocate(n);
	net->A = A;

	/* allocate the degrees vector */
	deg_vector = (int*)allocate(sizeof(int) * n);
	net->deg_vector = deg_vector;

	return net;
}

Network* create_network(FILE* input){
	Network* net;
	int n = 0;
	//int M = 0;
	int* row = NULL;
	int i = 0;
	int j = 0;
	int next_adj_edge = 0;
	int row_index = 0;

	/* read n from the file */
	n = int_fread(input);

	/* allocate the network and its members */
	net = allocate_network(n);
	net->n = n;

	/* allocate space for a row of the adjacency matrix */
	row = (int*)malloc(sizeof(int) * net->n);

	/* */
	for(i = 0; i < net->n; i++){
		net->deg_vector[i] = int_fread(input);
		for (j = 0; j < net->deg_vector[i]; j ++){
			//TODO: finish this! add row to sparse matrix !

		}
		spmat_add_row(net->A, row, i);
	}

	return net;
}
