/*
 * Group.c
 *
 */
#include "Group.h"
#include <stdlib.h>
#include "LibFuncsHandler.h"

Group* allocate_group(int n_g){
	Group* group = NULL;
	double* row_sums = NULL;

	/* Allocate group */
	group = (Group*)allocate(sizeof(group));

	/* Initiating vertices */
	group->vertices = NULL;
	group->n_g = 0;

	/* Initiating A_g and A_sums */
	group->A_g = spmat_allocate(n_g);
	row_sums = (double*)allocate(sizeof(double) * n_g);
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

void initiate_group(Group* group, spmat* A, double* row_sums, Node* g){
	group->A_g = A;
	group->row_sums = row_sums;
	group->vertices = g;
}
