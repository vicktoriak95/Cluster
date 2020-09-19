/*
 * Network.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "Network.h"
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
	for (i = 0; i < net->n; i++){
		net->deg_vector[i] = spmat_add_row_from_file(net->A, input, i);
	}

	/* Calculate the sum of all degrees in network */
	net->M = sum_of_integer_vector(net->deg_vector, net->n);
	if (net->M == 0){
		printf("M = 0, invalid graph");
		exit(EXIT_FAILURE);
	}

	return net;
}

void write_clusters_to_output(Old_Group* O, FILE* f, int n){
	Old_Group* group_head = O;
	Node* node_head = NULL;
	int group_length = 0;
	int node_length = 0;
	int outer_while_cnt = 0;
	int inner_while_cnt = 0;

	/* Calculating O group length and writing it to output */
	group_length = get_group_length(group_head, n);
	int_fwrite(group_length, f);

	/* Iterating over O and writing clusters to output */
	while (group_head != NULL){
		infinite_loop_detection(outer_while_cnt, n);
		node_length = get_node_length(group_head->vertices, n);
		int_fwrite(node_length, f);

		/* Iterating over a cluster and writing vertices in it to output */
		node_head = group_head->vertices;
		while (node_head != NULL){
			infinite_loop_detection(inner_while_cnt, n);
			int_fwrite(node_head->index, f);
			node_head = node_head->next;
			inner_while_cnt += 1;
		}

		group_head = group_head->next;
		outer_while_cnt += 1;
	}
}

void free_network(Network* net){
	spmat_free(net->A);
	free(net->deg_vector);
	free(net);
}

Network* network_from_args(spmat* matrix, int* deg_vector, int n, int M){
	Network* net = NULL;
	net = allocate_network(n);
	net->A = matrix;
	net->deg_vector = deg_vector;
	net->n = n;
	net->M = M;

	return net;
}
