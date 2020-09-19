/*
 * Group.c
 *
 */
#include "Group.h"
#include <stdlib.h>
#include "LibFuncsHandler.h"
#include "Network.h"
#include "LinearUtils.h"

Group* allocate_group(int n_g){
	Group* group = NULL;
	double* row_sums = NULL;
	int i = 0;

	/* Allocate group */
	group = (Group*)allocate(sizeof(Group));

	/* Initiating vertices */
	group->vertices = NULL;
	group->n_g = n_g;

	/* Initiating A_g and A_sums */
	group->A_g = spmat_allocate(n_g);
	row_sums = (double*)allocate(sizeof(double) * n_g);
	for(i = 0; i<n_g; i++){
		row_sums[i] = 0;
	}
	group->row_sums = row_sums;

	/* Initiating Next */
	group->next = NULL;

	return group;
}

void free_group(Group* group){
	spmat_free(group->A_g);
	free(group->row_sums);
	delete_node_list(group->vertices, group->n_g);
	free(group);
}

void initiate_group(Group* group, spmat* A, Node* g){
	group->A_g = A;
	group->vertices = g;
}

void divide_group(Network* N, Group* old_group, double* s, Group** new_group1, Group** new_group2){
	int n1 = 0;
	int n2 = 0;
	spmat* A1 = NULL;
	spmat* A2 = NULL;
	Node* vertices1 = old_group->vertices;
	Node* vertices2 = NULL;

	/* Calculating new group length */

	/* Allocating new Groups*/
	(*new_group1) = allocate_group(n1);
	(*new_group2) = allocate_group(n2);

	/* Dividing A */
	/*
	divide_spmat(old_group->A_g, s, &A1, &A2);*/

	/* Dividing vertices */
	vertices2 = divide_node_list(&vertices1, s, old_group->A_g->n);

	/* Calculate A_1 row_sums */
	B_row_sums(vertices1, N, (*new_group1)->row_sums, (*new_group1)->A_g->n);
	/* Calculate A_2 row_sums */
	B_row_sums(vertices2, N, (*new_group2)->row_sums, (*new_group2)->A_g->n);

	initiate_group((*new_group1), A1, vertices1);
	initiate_group((*new_group2), A2, vertices2);

	free_group(old_group);

}
