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
	/*
	Old_Group* P = NULL;
	Old_Group* O = NULL;
	Node* g = NULL;
	Node* g1 = NULL;
	Node* g2 = NULL;
	Old_Group* old_P = NULL;
	Node* head = NULL;
	int i = 0;
	int n_g = 0;*/
	int loop_cnt = 0;
	/*
	double* row_sums = NULL;
	*/
	double B_norm = 0;

	/* Read the input file into the net struct */
	create_network_and_first_group(input, &net, &group);

	push_group(&P, group);

	/* Create the group P with the first node */
	/*
	head = create_node(0);
	P = create_group(head);
	*/
	/* Create the other n-1 nodes and add them to the group P */
	/*
	for (i = 1; i < net->n; i ++){
		head->next = create_node(i);
		head = head->next;
	}
	*/

	/* Calculating g length and row_sums out of the loop for norm */
	/*
	g = P->vertices;
	n_g = get_node_length(g, net->n);
	row_sums = allocate(n_g * sizeof(double));
	B_row_sums(g, net, row_sums, n_g);
	*/

	/* Calculating Norm of matrix to be used for the whole run*/
	B_norm = Bhat_norm_new(net, group);
	/*
	B_norm = Bhat_norm(net, g, n_g);*/

	while (P != NULL){
		/* Number of iterations is linear in n */
		/* 2n chosen as an upper bound */
		infinite_loop_detection(loop_cnt, 2 * net->n);
		/* Pop g out of P */
		group = pop_group(&P);
		/*
		g = P->vertices;
		old_P = P;
		P = P->next;
		free(old_P);*/

		/* Calculating g length and row_sums */
		/*
		if(loop_cnt != 0){
			n_g = get_node_length(g, net->n);
			B_row_sums(g, net, row_sums, n_g);
		}*/

		s = (double*)allocate(group->A_g->n * sizeof(double));

		/* Divide g into two groups */
		/*
		devide_into_two(net, g, s, n_g, B_norm, row_sums);*/
		devide_into_two(net, group, s, B_norm);

		/* Maximizing Modularity */
		/*
		modularity_maximization(net, s, g, n_g, row_sums);*/
		modularity_maximization(net, group->A_g, s, group->vertices, group->A_g->n, group->row_sums);

		/*
		g1 = g;
		g2 = divide_node_list(&g1, s, n_g); */
		divide_group(net, group, s, &group1, &group2);
		free(s);
		/* If one of groups is empty, insert other into O, free empty group*/
		if((group1->A_g->n == 0) || (group2->A_g->n == 0)){
			if(group1->A_g->n == 0){
				push_group(&O, group2);
				free_group(group1);
			}
			else{
				push_group(&O, group1);
				free_group(group2);
			}
		}
		else { /* Both groups are not empty */
			/* For both groups push them into O iff size == 1 */
			if (group1->A_g->n == 1){
				push_group(&O, group1);
			}
			else {
				push_group(&P, group1);
			}
			if (group2->A_g->n == 1){
				push_group(&O, group2);
			}
			else {
				push_group(&P, group2);
			}
		}
		loop_cnt += 1;
	}

	/* Write the devision to output file */
	write_clusters_to_output(O, output, net->n);

	/* Free all */
	free_group_list(O, net->n);
	free_network(net);
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
	int i = 0;
	int n = old_group->A_g->n;
	/*
	spmat* A1 = NULL;
	spmat* A2 = NULL;
	*/
	Node* vertices1 = old_group->vertices;
	Node* vertices2 = NULL;

	/* Calculating new groups length */
	for (i=0; i<n; i++){
		if (s[i] == 1) {
			n1 += 1;
		}
	}
	n2 = n - n1;

	/* Allocating new Groups*/
	(*new_group1) = allocate_group(n1);
	(*new_group2) = allocate_group(n2);

	/* Dividing A */
	divide_spmat(old_group->A_g, s, &((*new_group1)->A_g), &((*new_group2)->A_g));
	/*
	print_sparse_matrix(A1);
	printf(" \n");
	print_sparse_matrix(A2);*/
	/*
	(*new_group1)->A_g = A1;
	(*new_group2)->A_g = A2;
	*/

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

void test_divide_group(){
	char* path = "C:\\Users\\User\\Dropbox\\Project\\טסטרים\\מדידות זמנים\\tester 4 3 1 4 0\\tester_binary.input";
	Network* net = NULL;
	Group* group = NULL;
	FILE* input = NULL;
	Group* group1 = NULL;
	Group* group2 = NULL;
	double s[4] = {1.0, -1.0, 1.0, -1.0};

	input = open_file(path, "rb");

	create_network_and_first_group(input, &net, &group);
	divide_group(net, group, s, &group1, &group2);
	printf("group1: \n");
	print_group(group1);
	printf("group2: \n");
	print_group(group2);
}
/*
int main(){
	test_divide_group();
	return 0;
}
*/
