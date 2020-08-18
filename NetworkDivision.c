/*
 * NetworkDevision.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#include <math.h>
#include <stdlib.h>
#include "NetworkDivision.h"
#include "NodeUtils.h"
#include "LinearUtils.h"
#include "LibFuncsHandler.h"
#include "Network.h"
#include "PowerIteration.h"

int main(int argc, char* argv[]){
	FILE* input;
	FILE* output;
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

	/* Read the input file into the net struct */
	input = open_file(argv[1], "rb");
	net = create_network(input);
	close_file(input);

	/*  Create the group P with the first node */
	head = create_node(0);
	P = create_group(head);
	/* Create the other n-1 nodes and add them to the group P */
	for (i = 1; i < net->n; i ++){
		head->next = create_node(i);
		head = head->next;
	}

	while (P != NULL){
		/* Pop g out of P */
		g = P->value;
		old_P = P;
		P = P->next;
		free(old_P);

		/* Divide g into two groups */
		n_g = get_node_length(g);
		s = (double*)allocate(n_g * sizeof(double));
		devide_into_two(net, g, s, n_g);
		modularity_maximization(net, s, g, n_g);
		g1 = g;
		g2 = divide_group(&g1, s, n_g);
		free(s);

		/* One of the groups is empty */
		if (g1 == NULL || g2 == NULL){
			/* set g as the non emply list */
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
	}

	/* Write the devision to output file */
	output = open_file(argv[2], "wb");
	write_clusters_to_output(O, output);
	close_file(output);

	print_output_file(argv[2]);

	/* Free all  */
	delete_group(O, net->n);
	free_network(net);

	return 0;
}

void indivisable(double* s, int n_g){
	int i;
	for (i = 0; i < n_g; i++){
		s[i] = 1.0;
	}
}


void devide_into_two(Network* N, Node* g, double* s, int n_g){
	double Q;
	double norm;
	double eigen_value;
	double* eigen_vector;

	/* Calculating matrix norm */
	norm = Bhat_norm(N, g, n_g);

	/* Finding biggest eigen_vector */
	eigen_vector = (double*)allocate(n_g * sizeof(double));
	eigen_vector = power_iteration(N, norm, g, n_g);
	eigen_value = Bhat_largest_eigenvalue(N, norm, eigen_vector, n_g, g);

	/* Calculate s - In case of non-positive eigenvalues do not divide */
	if (eigen_value <= 0){
		indivisable(s, n_g);
		return;
	}
	else{
		calculate_s(eigen_vector, s, n_g);
		Q = calc_Qk(N, s, g, n_g);
			if (Q <= 0){
				indivisable(s, n_g);
				return;
			}
	}
	free(eigen_vector);
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
			if (g1_head_prev == NULL) {
				*g1_p = g1_head->next;
				g1_head = g1_head->next;
			}
			else {
				g1_head_prev->next = g1_head->next;
				g1_head = g1_head_prev->next;
			}

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



double calc_Qk(Network* N, double* s, Node* g, int n_g){
	double res;
	double* result;

	result = (double*)allocate(n_g * sizeof(double));
	Bhat_multiplication(N, s, result, g, n_g);
	res = dot_product(s, result, n_g);
	free(result);

	return res;
}

void modularity_maximization(Network* N, double* s, Node* g, int n_g){
	int i;
	int k;
	int max_score_index = -1;
	int improve_index = 0;
	double Q0, Qk;
	double delta_Q;
	double Q_diff = - INFINITY;
	double max_score = - INFINITY;
	double* improve;
	int* indices;
	int* unmoved;

	/* Initiate unmoved with all the vertices' indexes corresponding to g */
	unmoved = (int*)allocate(n_g * sizeof(int));
	improve = (double*)allocate(n_g * sizeof(double));
	indices = (int*)allocate(n_g * sizeof(int));

	/* Keep improving while Q > 0 */
	do{
		/* Initiating unmoved with g values */
		vector_from_list(unmoved, g, n_g);

		/* Making n_g transitions of vertices to improve Q*/
		for (i = 0; i < n_g; i++){
			Q0 = calc_Qk(N, s, g, n_g);
			max_score = - INFINITY;
			max_score_index = -1;
			/* Searching for the best node to move among unmoved */
			for(k = 0; k < n_g; k++){
				if(unmoved[k] != (-1)){
					s[k] = s[k]*(-1);
					Qk = calc_Qk(N, s, g, n_g);
					Q_diff = Qk - Q0;
					if (Q_diff > max_score){
						max_score = Q_diff;
						max_score_index = k;
					}
					s[k] = s[k]*(-1);
				}
			}

			/* Move max_score_index to the other group */
			s[max_score_index] = s[max_score_index]*(-1);

			/* Add max_score_index to indices, which keeps the vertices transferring order */
			indices[i] = max_score_index;

			/* Update improve */
			if (i == 0){
				improve[i] = max_score;
			}
			else{
				improve[i] = improve[i-1] + max_score;
			}

			/* Remove max_score_index from unmoved */
			unmoved[max_score_index] = -1;
		}

		/* Find the largest improvement index */
		for (k = 0; k < n_g; k++){
			if (improve[k] > improve[improve_index]){
				improve_index = k;
			}
		}

		/* Move back all the remaining vertices that do not improve the modularity */
		for(i = n_g - 1; i > improve_index; i--){
			k = indices[i];
			s[k] = s[k]*(-1);
		}

		/* If the best separation occurs when all the vertices switched groups, then nothing actually changed */
		if (improve_index == n_g - 1){
			delta_Q = 0;
		}
		else{
			delta_Q = improve[improve_index];
		}


	}while(delta_Q > 0);

	free(unmoved);
	free(improve);
	free(indices);

}

void print_output_file(char *pathname){
	FILE* output_file = NULL;
	int num_of_groups = -1;
	int i = -1;
	int j = -1;
	int num_of_nodes_in_group = -1;
	int node_index = -1;

	output_file = open_file(pathname, "rb");
	num_of_groups = int_fread(output_file);
	/* Reading num of groups */
	printf("Num of groups: %d \n", num_of_groups);
	/* Reading num of nodes in every group, and nodes in group  */
	for(i=0; i < num_of_groups; i++){
		num_of_nodes_in_group = int_fread(output_file);
		printf("Num of nodes in group %d is %d \n", i, num_of_nodes_in_group);
		/* Reading nodes in group */
		printf("The nodes are:");
		for(j=0; j < num_of_nodes_in_group; j++){
			node_index = int_fread(output_file);
			printf(" %d", node_index);
		}
		printf(" \n");
	}
	close_file(output_file);

}


void test_modularity_maximization(){
	Node* g;
	int n = 4;
	int n_g = 4;
	int i;
	spmat* A;
	/*double* eigen_vector;*/
	Network* net;
	int M = 10;
	int deg_vector[4] = {2, 3, 2, 3};
	int matrix[4][4] = {{0, 1, 0, 1}, {1, 0, 1, 1}, {0, 1, 0, 1}, {1, 1, 1, 0}};
	int g_vector[4] = {0, 1, 2, 3};
	double s[4] = {0, 0, 0, 0};
	/* double norm = 0; */

	g = node_list_from_vector(g_vector, n_g);

	A = spmat_allocate(n);
	for(i = 0; i < n; i++){
		spmat_add_row_from_vector(A, matrix[i], i);
	}
	printf("created A \n");
	net = network_from_args(A, deg_vector, 4, M);

	/* ******** Checking norm, eigenvector, eigenvalue ************ */
	/* norm = Bhat_norm(net, g, n_g); */
	/* eigen_vector = power_iteration(net, norm, g, n_g); */
	/* print_vector(eigen_vector, n_g); */

	/* ******* Checking mod max ********* */
	/*
	modularity_maximization(net, s, g, n_g);
	printf("vector s is: ");
	print_vector(s, n_g);
	*/

	/* ****** Checking algo 2 *********** */
	devide_into_two(net, g, s, n_g);
	printf("vector s is: ");
	print_vector(s, n_g);
	/* TODO: Free things */

}

/*
int main(int argc, char* argv[]){
	test_modularity_maximization();

}
*/













