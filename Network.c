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

void create_network_and_first_group(FILE* input, Network** net, Group** group){
	int n = 0;
	int i = 0;
	int M = 0;
	Node* g_head = NULL;
	Node* g_pointer = NULL;

	/* Read n from the file */
	n = int_fread(input);

	/* Allocate the network and its members */
	*net = allocate_network(n);
	(*net)->n = n;

	/* Allocate Group */
	*group = allocate_group(n);

	/* Update the degrees vector and adjacency matrix from the file*/
	for (i = 0; i < n; i++){
		(*net)->deg_vector[i] = spmat_add_row_from_file((*group)->A_g, input, i);
		M += (*net)->deg_vector[i];

	}

	/* Creating Node list g with n-1 vertices */
	g_head = create_node(0);
	g_pointer = g_head;
	/* Create the other n-1 nodes and add them to the group P */
	for (i = 1; i < n; i ++){
		g_pointer->next = create_node(i);
		g_pointer = g_pointer->next;
	}

	/* Add Node list g to group */
	(*group)->vertices = g_head;

	/* Calculating row_sums for group */
	/*
	B_row_sums((*group)->vertices, *net, (*group)->row_sums, (*group)->n_g);
	*/
	/* Calculate the sum of all degrees in network */
	(*net)->M = M;
	/*
	(*net)->M = sum_of_integer_vector((*net)->deg_vector, net->n);
	*/
	if ((*net)->M == 0){
		printf("M = 0, invalid graph");
		exit(EXIT_FAILURE);
	}

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

void test_create_net_first_group(){
	char* path = "C:\\Users\\User\\Dropbox\\Project\\טסטרים\\מדידות זמנים\\tester 4 3 1 4 0\\tester_binary.input";
	Network* net = NULL;
	Group* group = NULL;
	FILE* input = NULL;

	input = open_file(path, "rb");

	create_network_and_first_group(input, &net, &group);
	printf("n is: %d \n", net->n);
	printf("n is: %d \n", group->n_g);
	printf("# Network vals: # \n");
	printf("deg vector: \n");
	print_int_vector(net->deg_vector, net->n);
	printf("M is: %d \n", net->M);
	printf("# Group vals: # \n");
	printf("A_g is: \n");
	print_sparse_matrix(group->A_g);
	printf("nodes list is: \n");
	print_node_list(group->vertices);
	printf("row sums are: \n");
	print_vector(group->row_sums, group->n_g);
	free_network(net);
	free_group(group);
	printf("Vicki is the best");
}

int main(){
	test_create_net_first_group();
	return 0;
}
