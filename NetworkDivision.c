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


int main(FILE* input, FILE* output){
	Network* net = NULL;
	Group* P = NULL;
	Group* O = NULL;
	Node* g = NULL;
	Node* g1 = NULL;
	Node* g2 = NULL;
	double* s = NULL;
	Group* old_P = NULL;
	Group* tmp_O = NULL;
	Group* tmp_P = NULL;
	Node* head = NULL;
	int i = 0;
	int n_g = 0;


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
			// set g as the non emply list
			g1 != NULL ? g = g1 : g = g2;
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
	write_clusters_to_output(O, output);

	/* Free all  */
	delete_group(&O, net->n);
	free_network(net);

	return 0;
}


/*
int* devide_into_two(Network* N, Node* g){

}
*/

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
	double * result;

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
	Node* g_p = g;


	/* Initiate unmoved with all the vertices' indexes corresponding to g */
	unmoved = (int*)allocate(n_g * sizeof(int));
	for(i = 0; i < n_g; i++){
		unmoved[i] = g_p->index;
		g_p = g_p->next;
	}

	improve = (double*)allocate(n_g * sizeof(double));
	indices = (int*)allocate(n_g * sizeof(int));

	do{
		for (i = 0; i < n_g; i++){
			Q0 = calc_Qk(N, s, g, n_g);

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

}



void test_modularity_maximization(){
	Node* g;
	int n = 4;
	int n_g = 3;
	int i;
	spmat* A;
	/*double* eigen_vector;*/
	Network* net;
	int M = 8;
	int deg_vector[4] = {1, 3, 2, 2};
	int matrix[4][4] = {{0, 1, 0, 0}, {1, 0, 1, 1}, {0, 1, 0, 1}, {0, 1, 1, 0}};
	int g_vector[4] = {0, 1, 2, 3};
	double s[3] = {1, -1, 1};
	/* double norm = 0; */


	g = node_list_from_vector(g_vector, n_g);

	A = spmat_allocate(n);
	for(i = 0; i < n; i++){
		spmat_add_row_from_vector(A, matrix[i], i);
	}
	printf("created A \n");

	net = network_from_args(A, deg_vector, 4, M);
	modularity_maximization(net, s, g, n_g);
	/* norm = Bhat_norm(net, g, n_g); */
	/* eigen_vector = power_iteration(net, norm, g, n_g); */
	/* print_vector(eigen_vector, n_g); */

	/* TODO: Free things */

}


int main(int argc, char* argv[]){
	test_modularity_maximization();
	printf("finished - Adi is the best");

}











