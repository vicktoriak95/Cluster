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
	net->M = sum_of_integer_vector(net->deg_vector, net->n);
	if (net->M == 0){
		printf("M = 0, invalid graph");
		exit(1);
	}

	return net;
}

void write_clusters_to_output(Group* O, FILE* f){
	Group* group_head = O;
	Node* node_head = NULL;
	int group_length = 0;
	int node_length = 0;

	group_length = get_group_length(group_head);
	int_fwrite(group_length, f);

	while (group_head != NULL){
		node_length = get_node_length(group_head->value);
		int_fwrite(node_length, f);

		node_head = group_head->value;
		while (node_head != NULL){
			int_fwrite(node_head->index, f);
			node_head = node_head->next;
		}

		group_head = group_head->next;
	}
}

/* Free all allocated space of a network */
void free_network(Network* net){
	spmat_free(net->A);
	free(net->deg_vector);
	free(net);
}

/* Creates network from args */
Network* network_from_args(spmat* matrix, int* deg_vector, int n, int M){
	Network* net = NULL;
	net = allocate_network(n);
	net->A = matrix;
	net->deg_vector = deg_vector;
	net->n = n;
	net->M = M;

	return net;
}
