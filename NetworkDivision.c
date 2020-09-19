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
#include "Group.h"

void divide_net_to_clusters(FILE* input, FILE* output){
	Network* net = NULL;
	Group* group = NULL;
	Group* P = NULL;
	Group* O = NULL;
	Group* group1 = NULL;
	Group* group2 = NULL;
	double* s = NULL;
	int loop_cnt = 0;
	double B_norm = 0;

	/* Read the input file into the net struct */
	create_network_and_first_group(input, &net, &group);

	/* Push first group into P */
	push_group(&P, group);

	/* Calculating Norm of matrix to be used for the whole run*/
	B_norm = Bhat_norm_new(net, group);

	while (P != NULL){
		/* Number of iterations is linear in n */
		/* 2n chosen as an upper bound */
		infinite_loop_detection(loop_cnt, 2 * net->n);

		/* Pop g out of P */
		group = pop_group(&P);

		s = (double*)allocate(group->A_g->n * sizeof(double));

		/* Divide g into two groups */
		devide_into_two(net, group, s, B_norm);

		/* Maximizing Modularity */
		modularity_maximization(net, group->A_g, s, group->vertices, group->A_g->n, group->row_sums);

		/* Dividing group by s */
		divide_group(net, group, s, &group1, &group2);
		free(s);

		/* Push groups into O and P */
		groups_into_O_P(&O, &P, &group1, &group2);
		loop_cnt += 1;
	}

	/* Write the devision to output file */
	write_clusters_to_output(O, output, net->n);

	/* Free all */
	free_group_list(O, net->n);
	free_network(net);
}

void groups_into_O_P(Group** O, Group** P, Group** group1, Group** group2){
	/* If one of groups is empty, insert other into O, free empty group*/
	if(((*group1)->A_g->n == 0) || ((*group2)->A_g->n == 0)){
		if((*group1)->A_g->n == 0){
			push_group(O, (*group2));
			free_group((*group1));
		}
		else{
			push_group(O, (*group1));
			free_group(*group2);
		}
	}
	else { /* Both groups are not empty */
		/* For both groups push them into O iff size == 1 */
		if ((*group1)->A_g->n == 1){
			push_group(O, (*group1));
		}
		else {
			push_group(P, (*group1));
		}
		if ((*group2)->A_g->n == 1){
			push_group(O, (*group2));
		}
		else {
			push_group(P, (*group2));
		}
	}
}

void indivisable(double* s, int n_g){
	int i = 0;

	for (i = 0; i < n_g; i++){
		s[i] = 1.0;
	}
}

void devide_into_two(Network* N, Group* group, double* s, double B_norm){
	double Q = 0;
	double norm = B_norm;
	double eigen_value = 0;
	double* eigen_vector = NULL;
	int n_g = group->A_g->n;
	double* row_sums = group->row_sums;
	Node* g = group->vertices;
	spmat* A = group->A_g;

	/* Finding biggest eigen_vector */
	eigen_vector = power_iteration(N, A, norm, g, n_g, row_sums);
	eigen_value = Bhat_largest_eigenvalue(N, A, norm, eigen_vector, n_g, g, row_sums);

	calculate_s(eigen_vector, s, n_g);

	free(eigen_vector);

	/* Calculate s - In case of non-positive eigenvalues do not divide */
	if (eigen_value <= 0){
		indivisable(s, n_g);
	}
	else{
		Q = calc_Qk(N, A, s, g, n_g, row_sums);
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

void divide_group(Network* N, Group* old_group, double* s, Group** new_group1, Group** new_group2){
	int n1 = 0;
	int n2 = 0;
	int n = old_group->A_g->n;
	Node* vertices1 = old_group->vertices;
	Node* vertices2 = NULL;

	/* Calculating new groups length */
	n1 = calc_group_length(s, n);
	n2 = n - n1;
	/* Allocating new Groups*/
	(*new_group1) = allocate_group(n1);
	(*new_group2) = allocate_group(n2);

	/* Dividing A */
	divide_spmat(old_group->A_g, s, &((*new_group1)->A_g), &((*new_group2)->A_g));

	/* Dividing vertices */
	vertices2 = divide_node_list(&vertices1, s, old_group->A_g->n);
	(*new_group1)->vertices = vertices1;
	(*new_group2)->vertices = vertices2;

	/* Calculate A_1 row_sums */
	B_row_sums_new((*new_group1), N);
	/* Calculate A_2 row_sums */
	B_row_sums_new((*new_group2), N);

	old_group->vertices = NULL;
	free_group(old_group);

}

/* Calculating new groups length */
int calc_group_length(double* s, int n){
	int i = 0;
	int n1 = 0;

	for (i=0; i<n; i++){
		if (s[i] == 1) {
			n1 += 1;
		}
	}
	return n1;
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
