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
	delete_node_list(group->vertices, group->A_g->n);
	spmat_free(group->A_g);
	free(group->row_sums);
	free(group);
}

void push_group(Group** head_ref, Group* curr_group){
	if ((*head_ref) == NULL) {
		(*head_ref) = curr_group;
	}
	else {
		/* Make next of new list as head */
		curr_group->next = (*head_ref);
		/* Move the head to point to the new list*/
		(*head_ref) = curr_group;
	}
}

Group* pop_group(Group** head_ref){
	Group* group = NULL;

	group = (*head_ref);
	(*head_ref) = (*head_ref)->next;
	return group;
}

int get_group_length(Group* group, int n){
	Group* head = group;
	int length = 0;
	int cnt = 0;

	while (head != NULL){
		infinite_loop_detection(cnt, n);
		length += 1;
		head = head->next;
		cnt += 1;
	}
	return length;
}

void free_group_list(Group* head_ref, int n){
	int cnt = 0;
	Group* current = head_ref;
	Group* next = NULL;

	while (current != NULL){
		infinite_loop_detection(cnt, n);
		next = current->next;
		free_group(current);
		current = next;
		cnt += 1;
	}
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
