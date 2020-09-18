/*
 * NetworkDevision.c
 *
 */

#include <math.h>
#include <stdlib.h>
#include "NetworkDivision.h"
#include "LinearUtils.h"
#include "LibFuncsHandler.h"
#include "PowerIteration.h"
#include "ModularityMax.h"

void divide_net_to_clusters(FILE* input, FILE* output){
	Network* net = NULL;
	Group* P = NULL;
	Group* O = NULL;
	Node* g = NULL;
	Node* g1 = NULL;
	Node* g2 = NULL;
	double* s = NULL;
	Group* old_P = NULL;
	Node* head = NULL;
	int i = 0;
	int n_g = 0;
	int loop_cnt = 0;
	double* row_sums = NULL;
	double B_norm = 0;

	/* Read the input file into the net struct */
	net = create_network(input);

	/*  Create the group P with the first node */
	head = create_node(0);
	P = create_group(head);
	/* Create the other n-1 nodes and add them to the group P */
	for (i = 1; i < net->n; i ++){
		head->next = create_node(i);
		head = head->next;
	}

	/* Calculating g length and row_sums out of the loop for norm */
	g = P->value;
	n_g = get_node_length(g, net->n);
	row_sums = allocate(n_g * sizeof(double));
	B_row_sums(g, net, row_sums, n_g);

	/* Calculating Norm  of matrix to be used for the whole run*/
	B_norm = Bhat_norm(net, g, n_g);

	while (P != NULL){
		/* Number of iterations is linear in n */
		/* 2n chosen as an upper bound */
		infinite_loop_detection(loop_cnt, 2 * net->n);
		/* Pop g out of P */
		g = P->value;
		old_P = P;
		P = P->next;
		free(old_P);

		/* Calculating g length and row_sums */
		if(loop_cnt != 0){
			n_g = get_node_length(g, net->n);
			row_sums = allocate(n_g * sizeof(double));
			B_row_sums(g, net, row_sums, n_g);
		}
		s = (double*)allocate(n_g * sizeof(double));

		/* Divide g into two groups */
		devide_into_two(net, g, s, n_g, B_norm, row_sums);

		/* Maximizing Modularity */
		modularity_maximization(net, s, g, n_g, row_sums);

		g1 = g;
		g2 = divide_group(&g1, s, n_g);
		free(s);

		/* One of the groups is empty */
		if (g1 == NULL || g2 == NULL){
			/* set g as the non empty list */
			if (g1 != NULL){
				g = g1;
			}
			else {
				g = g2;
			}
			/* Insert g into O */
			push_group(&O, g);
		}
		else {
			/* For each group in {g1, g2} push them into O iff size == 1 */
			if (g1->next == NULL){
				push_group(&O, g1);
			}
			else {
				push_group(&P, g1);
			}
			if (g2->next == NULL){
				push_group(&O, g2);
			}
			else {
				push_group(&P, g2);
			}
		}
		loop_cnt += 1;
	}

	/* Write the devision to output file */
	write_clusters_to_output(O, output, net->n);

	/* Free all */
	delete_group(O, net->n);
	free_network(net);
	free(row_sums);
}

void indivisable(double* s, int n_g){
	int i = 0;

	for (i = 0; i < n_g; i++){
		s[i] = 1.0;
	}
}

void devide_into_two(Network* N, Node* g, double* s, int n_g, double B_norm, double* row_sums){
	double Q = 0;
	double norm = B_norm;
	double eigen_value = 0;
	double* eigen_vector = NULL;

	/* Finding biggest eigen_vector */
	eigen_vector = power_iteration(N, norm, g, n_g, row_sums);
	eigen_value = Bhat_largest_eigenvalue(N, norm, eigen_vector, n_g, g, row_sums);

	calculate_s(eigen_vector, s, n_g);

	free(eigen_vector);

	/* Calculate s - In case of non-positive eigenvalues do not divide */
	if (eigen_value <= 0){
		indivisable(s, n_g);
	}
	else{
		Q = calc_Qk(N, s, g, n_g, row_sums);
			if (Q <= 0){
				indivisable(s, n_g);
			}
	}
}

void calculate_s(double* eigen_vector, double* s, int n_g){
	int i = 0;

	for (i = 0; i < n_g; i ++){
		if (eigen_vector[i] >= 0){
			s[i] = 1;
		}
		else {
			s[i] = -1;
		}
	}
}

Node* divide_group(Node** g1_p, double* s, int n_g){
	Node* g2 = NULL;
	Node* g1_head = *g1_p;
	Node* g1_head_prev = NULL;
	Node* g2_head = g2;
	Node* to_be_g2_head = NULL;
	int i = 0;

	/* Iterating over s */
	for (i = 0; i < n_g; i ++){
		/* If we do not move the node from g1 */
		if (s[i] > 0){
			g1_head_prev = g1_head;
			g1_head = g1_head->next;
		}
		/* If we move the node to g2 */
		else {
			to_be_g2_head = g1_head;
			/* If we are moving first node from g1 */
			if (g1_head_prev == NULL) {
				*g1_p = g1_head->next;
				g1_head = g1_head->next;
			}
			else {
				g1_head_prev->next = g1_head->next;
				g1_head = g1_head_prev->next;
			}
			/* If we are moving first node to g_2 */
			if (g2_head == NULL){
				g2 = to_be_g2_head;
				g2_head = to_be_g2_head;
			}
			else {
				g2_head->next = to_be_g2_head;
				g2_head = g2_head->next;
			}
			g2_head->next = NULL;
		}
	}
	return g2;
}

void print_output_file(FILE* output_file){
	int num_of_groups = -1;
	int i = -1;
	int j = -1;
	int num_of_nodes_in_group = -1;
	int node_index = -1;

	num_of_groups = int_fread(output_file);
	/* Reading num of groups */
	printf("Num of groups: %d \n", num_of_groups);
	/* Reading num of nodes in every group, and nodes in group  */
	for (i = 0; i < num_of_groups; i++){
		num_of_nodes_in_group = int_fread(output_file);
		printf("Num of nodes in group %d is %d \n", i, num_of_nodes_in_group);
		/* Reading nodes in group */
		printf("The nodes are:");
		for (j = 0; j < num_of_nodes_in_group; j++){
			node_index = int_fread(output_file);
			printf(" %d", node_index);
		}
		printf(" \n");
	}
}
