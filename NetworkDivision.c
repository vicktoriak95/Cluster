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

void divide_net_to_clusters(FILE* input, FILE* output, clock_t start){
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
	int cnt = 0;
	double* row_sums = NULL;
	double B_norm = 0;
	clock_t before_devide_into_two, after_devide_into_two, after_modularity_maximization;

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

	/* TODO: make look good*/
	g = P->value;
	n_g = get_node_length(g, net->n);
	row_sums = allocate(n_g * sizeof(double));
	B_row_sums(g, net, row_sums, n_g);
	B_norm = Bhat_norm(net, g, net->n, row_sums);

	while (P != NULL){
		/* Number of iterations is linear in n */
		/* 2n chosen as an upper bound */
		infinite_loop_detection(cnt, 2 * net->n);
		/* Pop g out of P */
		g = P->value;
		old_P = P;
		P = P->next;
		free(old_P);

		/* Count g length */
		n_g = get_node_length(g, net->n);
		row_sums = allocate(n_g * sizeof(double));
		B_row_sums(g, net, row_sums, n_g);
		s = (double*)allocate(n_g * sizeof(double));

		/* Divide g into two groups */
		before_devide_into_two = clock();
		printf("Time up to before_devide_into_two: %f seconds\n", ((double)(before_devide_into_two-start) / CLOCKS_PER_SEC));
		devide_into_two(net, g, s, n_g, start, B_norm, row_sums);
		after_devide_into_two = clock();
		printf("Time up to after_devide_into_two: %f seconds\n", ((double)(after_devide_into_two-start) / CLOCKS_PER_SEC));

		modularity_maximization(net, s, g, n_g, row_sums);
		after_modularity_maximization = clock();
		printf("Time up to after_modularity_maximization: %f seconds\n\n", ((double)(after_modularity_maximization-start) / CLOCKS_PER_SEC));

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
		cnt += 1;
	}

	/* Write the devision to output file */
	write_clusters_to_output(O, output, net->n);

	/* Free all */
	delete_group(O, net->n);
	free_network(net);
}

void indivisable(double* s, int n_g){
	int i = 0;

	for (i = 0; i < n_g; i++){
		s[i] = 1.0;
	}
}

void devide_into_two(Network* N, Node* g, double* s, int n_g, clock_t start, double B_norm, double* row_sums){
	double Q = 0;
	double norm = B_norm;
	double eigen_value = 0;
	double* eigen_vector = NULL;
	/*
	clock_t before_norm, after_norm;
	*/
	/*clock_t after_power_iteration, after_largest_eigenvalue, after_calculating_s, finish;*/

	/* Calculating matrix norm */
	/*
	before_norm = clock();
	printf("Time up to norm: %f seconds\n", ((double)(before_norm-start) / CLOCKS_PER_SEC));
	norm = Bhat_norm(N, g, n_g);
	after_norm = clock();
	printf("Time up to after norm: %f seconds\n", ((double)(after_norm-start) / CLOCKS_PER_SEC));
	*/
	/*printf("### Entered into divide into two ###\n");*/
	/* Finding biggest eigen_vector */
	eigen_vector = power_iteration(N, norm, g, n_g, row_sums);
	/*after_power_iteration = clock();
	printf("Time up to after power iteration: %f seconds\n", ((double)(after_power_iteration-start) / CLOCKS_PER_SEC));*/

	eigen_value = Bhat_largest_eigenvalue(N, norm, eigen_vector, n_g, g, row_sums);
	/*after_largest_eigenvalue = clock();
	printf("Time up to largest eigenvalue: %f seconds\n", ((double)(after_largest_eigenvalue-start) / CLOCKS_PER_SEC));*/

	calculate_s(eigen_vector, s, n_g);
	/*after_calculating_s = clock();
	printf("Time up to after_calculating_s: %f seconds\n", ((double)(after_calculating_s-start) / CLOCKS_PER_SEC));*/

	free(eigen_vector);
	/*finish = clock();
	printf("Time up to after finish: %f seconds\n", ((double)(finish-start) / CLOCKS_PER_SEC));
	printf("### Exited divide into two ###\n");*/

	/* Calculate s - In case of non-positive eigenvalues do not divide */
	if (eigen_value <= 0){
		indivisable(s, n_g);
		return;
	}
	else{
		Q = calc_Qk(N, s, g, n_g, row_sums);
			if (Q <= 0){
				indivisable(s, n_g);
				return;
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

double calc_Qk(Network* N, double* s, Node* g, int n_g, double* row_sums){
	double res = 0;
	double* result = NULL;

	result = (double*)allocate(n_g * sizeof(double));
	Bhat_multiplication(N, s, result, g, n_g, row_sums);
	res = dot_product(s, result, n_g);
	free(result);

	return res;
}


double calc_Q_diff(double* d, int i, Network* N, Node* g, int n_g, double A_sum){
	double second_sum = 0;
	double res = 0;
	Node* g_pointer = g;
	double to_add = 0;
	int j = 0;
	int real_j = 0;
	int real_i = 0;

	real_i = get_node_value(g, i);

	for(j = 0; j < n_g; j++){
		real_j = g_pointer->index;
		to_add = (((double)N->deg_vector[real_i] * N->deg_vector[real_j]) / N->M) * d[j];
		second_sum += to_add;
		g_pointer = g_pointer->next;
	}
	res = 4 * d[i] * (A_sum - second_sum) + 4 * (((double)N->deg_vector[real_i] * N->deg_vector[real_i])) / N->M;

	return res;
}

void update_A_sums(double* A_sums, int k, int real_k, Network* N, double* s, Node* g){
	Node_matrix* row_head = NULL;
	Node* g_head = g;
	int cnt = 0;
	int mat_col_index = 0;
	int g_col_index = 0;
	int vector_index = 0;
	spmat* A = N->A;

	row_head = ((Node_matrix** )A->private)[real_k];

	/* Iterating over row, summing only entries in g */
	while ((row_head != NULL) && (g_head != NULL)){
		infinite_loop_detection(cnt, N->n);

		/* Comparing indices and promoting g_head, mat_head respectively */
		mat_col_index = row_head->col_index;
		g_col_index = g_head->index;
		if (mat_col_index == g_col_index){
			A_sums[vector_index] -= 2 * row_head->value * s[k];
			g_head = g_head->next;
			row_head = (Node_matrix*)row_head->next;
			vector_index += 1;
		}
		else if (mat_col_index > g_col_index){
			g_head = g_head->next;
			vector_index += 1;
		}
		else {
			row_head = (Node_matrix*)row_head->next;
		}
		cnt += 1;
	}
}


void modularity_maximization(Network* N, double* s, Node* g, int n_g, double* row_sums){
	int i = 0;
	int k = 0;
	int max_score_index = -1;
	int improve_index = 0;
	double delta_Q = 0;
	double Q_diff = 0;
	double max_score = 0;
	int* indices = NULL;
	int* unmoved = NULL;
	int cnt = 0;
	int first_score = 1;
	int power_of_2 = n_g;
	int* next_unmoved = NULL;
	Node* g_unmoved_head = g;
	int* temp;
	double* A_sums = NULL;
	int real_k = 0;
	int real_max_score_index = 0;
	double curr_improve = 0;
	double max_improve = -HUGE_VAL;
	double Q_max = 0;
	double Q_0;

	printf("got into mod_max bitches \n");

	/* Initiate unmoved with all the vertices indexes corresponding to g */
	unmoved = (int*)allocate(n_g * sizeof(int));
	indices = (int*)allocate(n_g * sizeof(int));
	next_unmoved = (int*)allocate(n_g * sizeof(int));
	A_sums = (double*)allocate(n_g * sizeof(double));

	/* Initiating unmoved with g values */
	vector_from_list(unmoved, g, n_g);

	/* Keep improving while Q > 0 */
	do{
		if (n_g > INT_MAX_POWER_OF_2){
			power_of_2 = INT_MAX_POWER_OF_2;
		}
		/* Every iteration increases the Q of the division and thus we would not visit the same division twice
		 * There are at most (2 ** n_g) divisions */
		infinite_loop_detection(cnt, pow(2, power_of_2));

		/* Calculating Aux sums */
		 A_row_sums(g, N, A_sums, n_g, s);

		/* Initiating unmoved with g values */
		/*
		vector_from_list(unmoved, g, n_g);
		*/

		/* Initiating improve variabales */
		max_improve = -HUGE_VAL;
		improve_index = -1;

		/* Calc Q_0*/
		Q_0 = calc_Qk(N, s, g, n_g, row_sums);

		/* Making n_g transitions of vertices to improve Q */
		for (i = 0; i < n_g; i++){

			/* Updating next unmoved */
			next_unmoved[i] = g_unmoved_head->index;
			g_unmoved_head = g_unmoved_head->next;

			max_score_index = -1;
			first_score = 1;
			/* Searching for the best node to move among unmoved */
			for(k = 0; k < n_g; k++){
				if(unmoved[k] != (-1)){
					s[k] = s[k] * (-1);
					real_k = get_node_value(g, k);
					Q_diff = calc_Q_diff(s, k, N, g, n_g, A_sums[k]);
					if ((first_score == 1) || (Q_diff > max_score)){
						first_score = 0;
						max_score = Q_diff;
						max_score_index = k;
						real_max_score_index = real_k;
						Q_max = Q_0 + Q_diff;
					}
					s[k] = s[k] * (-1);
				}
			}

			/* Update A sums */
			update_A_sums(A_sums, max_score_index, real_max_score_index, N, s ,g);

			/* Move max_score_index to the other group */
			s[max_score_index] = s[max_score_index]*(-1);

			/* Add max_score_index to indices, which keeps the vertices transferring order */
			indices[i] = max_score_index;

			/* Update improve */
			curr_improve += Q_max - Q_0;
			if(max_improve < curr_improve){
				max_improve = curr_improve;
				improve_index = i;
			}

			/* Remove max_score_index from unmoved */
			unmoved[max_score_index] = -1;
			Q_0  = Q_max;
		}

		/* If the best separation occurs when all the vertices switched groups, then nothing actually changed */
		if (improve_index == n_g - 1){
			delta_Q = 0;
		}
		else{
			delta_Q = max_improve;
			/* Move back all the remaining vertices that do not improve the modularity */
			for(i = n_g - 1; i > improve_index; i--){
				k = indices[i];
				s[k] = s[k]*(-1);
			}
		}

		/* Switching unmoved and unmoved next*/
		temp = unmoved;
		unmoved = next_unmoved;
		next_unmoved = temp;
		g_unmoved_head = g;


		cnt += 1;
	} while(delta_Q > 0);

	free(unmoved);
	free(indices);
	free(next_unmoved);
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
