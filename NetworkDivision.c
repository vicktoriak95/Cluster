/*
 * NetworkDevision.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#include "NetworkDivision.h"
#include "NodeUtils.h"
#include "LibFuncsHandler.h"

#include <stdlib.h>
/*
int main(FILE* input, FILE* output){
	return 0;
}
*/

void write_clusters_to_output(Group* O, FILE* f){
	Group* group_head = O;
	Node* node_head = NULL;
	int group_length = 0;
	int node_length = 0;

	group_length = get_group_length(group_head);
	int_fwrite(group_length, f);

	while (group_head->next != NULL){
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

/*
int* devide_into_two(Network* N, Node* g){

}
*/

void calculate_s(double* eigen_vector, double* s, int n){
	int i = 0;
	for (i = 0; i < n; i ++){
		if (eigen_vector[i] >= 0){
			s[i] = 1;
		}
		else {
			s[i] = -1;
		}
	}
}

/*
Node* divide_group(Node** g1_p, int* s, int n){
	Node* g2 = NULL;
	Node* g1_head = *g1_p;
	Node* g2_head = g2;
	int i = 0;

	while (i < n && s[i] > 0) {
		g1_head = g1_head->next;
		i ++;
	}

	return g2;
}
*/
/*
void modularity_maximization(int* s, Node* g, int n_g){

}
*/
