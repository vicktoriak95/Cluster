/*
 * Network.c
 *
 */

#include <stdio.h>
#include <stdlib.h>

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

void write_clusters_to_output(Group* O, FILE* f, int n){
	Group* group_head = O;
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
		node_length = get_node_length(group_head->value, n);
		int_fwrite(node_length, f);

		/* Iterating over a cluster and writing vertices in it to output */
		node_head = group_head->value;
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

double B_row_sum(spmat* A, int mat_row_index, Node* g, Network* N){
	Node_matrix* row_head = NULL;
	Node* g_col_head = g;
	double row_sum = 0;
	int loop_cnt = 0;
	int mat_col_index = 0;
	int g_col_index = 0;
	int* deg_vector = N->deg_vector;
	int M = N->M;
	
	row_head = ((Node_matrix** )A->private)[mat_row_index];

	/* Iterating over row, summing only entries in g */
	while ((row_head != NULL) && (g_col_head != NULL)){
		infinite_loop_detection(loop_cnt, A->n);

		/* Comparing indices and promoting g_head, mat_head respectively */
		mat_col_index = row_head->col_index;
		g_col_index = g_col_head->index;
		/* If we are looking at entry in g and it is not 0, adding 1-k_i*k_j/M to row_sum */
		if (mat_col_index == g_col_index){
			row_sum += row_head->value - ((double)deg_vector[g_col_index] * deg_vector[mat_row_index]) / M;
			g_col_head = g_col_head->next;
			row_head = (Node_matrix*)row_head->next;
		}
		/* If we are looking at entry in g that is 0, adding -k_i*k_j/M to row_sum */
		else if(mat_col_index > g_col_index){
			row_sum -= ((double)deg_vector[g_col_index] * deg_vector[mat_row_index]) / M;
			g_col_head = g_col_head->next;
		}
		/* If we are looking at entry not in g, adding nothing to row_sum */
		else{
			row_head = (Node_matrix*)row_head->next;
		}
		loop_cnt += 1;
	}
	/* If there are entries in A with value 0 at the end of the row, adding -k_i*k_j/M to row_sum */
	while(g_col_head != NULL){
		g_col_index = g_col_head->index;
		row_sum -= ((double)deg_vector[g_col_index] * deg_vector[mat_row_index]) / M;
		g_col_head = g_col_head->next;
	}
	return row_sum;
}

void B_row_sums(Node* g, Network* N, double* row_sums, int n_g){
	Node* g_row_head = g;
	double row_sum = 0;
	int mat_row_index = 0;
	int i = 0;

	/* Calculating sum for each row */
	for(i=0; i<n_g; i++){
		/* Find row index in A*/
		mat_row_index = g_row_head->index;

		row_sum = B_row_sum(N->A, mat_row_index, g, N);

		row_sums[i] = row_sum;
		g_row_head = g_row_head->next;
	}
}

void test_row_sum(){
	Node* g;
	int n = 4;
	int n_g = 3;
	int i;
	spmat* A;
	Network* net;
	int deg_vector[4] = {2, 2, 3, 1};
	int matrix[4][4] = {{0, 1, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 1}, {0, 0, 0, 1}};
	int g_vector[3] = {0, 1, 3};
	int M = 8;
	int row_num = 1;
	double row_sum = 0;
	double* row_sums = NULL;


	g = node_list_from_vector(g_vector, n_g);

	A = spmat_allocate(n);
	for(i = 0; i < n; i++){
		spmat_add_row_from_vector(A, matrix[i], i);
	}
	printf("created A \n");

	net = network_from_args(A, deg_vector, n, M);

	row_sum = B_row_sum(A, row_num, g, net);
	printf("Row sum is: %f \n", row_sum);

	row_sums = allocate(n_g);
	B_row_sums(g, net, row_sums, n_g);
	printf("Row sums: \n");
	print_vector(row_sums, n_g);
}



/*
int main(){
	test_row_sum();
}
*/

