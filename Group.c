/*
 * Group.c
 *
 */
#include "Group.h"
#include <stdlib.h>
#include "LibFuncsHandler.h"
#include "LinearUtils.h"
#include "SparseMatrix.h"

Group* allocate_group(int n_g){
	Group* group = NULL;
	double* row_sums = NULL;
	int i = 0;

	/* Allocate group */
	group = (Group*)allocate(sizeof(Group));

	/* Initiating vertices */
	group->vertices = NULL;

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
	delete_node_list(group->vertices, group->A_g->n);
	free(group);
}

void print_group(Group* group){
	printf("n is: %d \n", group->A_g->n);
	printf("A_g is: \n");
	print_sparse_matrix(group->A_g);
	printf("nodes list is: \n");
	print_node_list(group->vertices);
	printf("row sums are: \n");
	print_vector(group->row_sums, group->A_g->n);
}
