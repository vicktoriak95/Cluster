#include <stdlib.h>
#include <stdio.h>

#include "Network.h"
#include "SparseMatrix.h"
#include "LibFuncsHandler.h"
#include "LinearUtils.h"

Network* allocate_network(int n){
	Network* net = NULL;
	spmat* A = NULL;
	int* deg_vector = NULL;

	/* Allocate the network */
	net = (Network*)allocate(sizeof(Network));

	/* Allocate the adjacency matrix */
	A = spmat_allocate(n);
	net->A = A;

	/* Allocate the degrees vector */
	deg_vector = (int*)allocate(sizeof(int) * n);
	net->deg_vector = deg_vector;

	return net;
}

Network* create_network(FILE* input){
	Network* net = NULL;
	int n = 0;
	int i = 0;

	/* Read n from the file */
	n = int_fread(input);

	/* Allocate the network and its members */
	net = allocate_network(n);
	net->n = n;

	/* Update the degrees vector and adjacency matrix from the file*/
	for(i = 0; i < net->n; i++){
		net->deg_vector[i] = spmat_add_row_from_file(net->A, input, i);
	}

	/* Calculate the sum of all degrees in network */
	net->M = sum_of_vector(net->deg_vector, net->n);

	return net;
}
