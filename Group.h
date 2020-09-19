/*
 * Group.h
 *
 * Definition of GROUP Struct which represents Group in network.
 * Group is a potential cluster, and is being divided in net into cluster algorithm.
 * Members:
 * vertices: Node list representing vertices in group.
 * A_g : Sub-matrix of Adjacency matrix according to group.
 * row_sums: auxiliary sums used in multiplication.
 *
 * Functions: Allocating, freeing and creating groups, and classic node-like funcs like push and pop
 *
 */

#ifndef GROUP_H_
#define GROUP_H_
#include "SparseMatrix.h"
#include "NodeUtils.h"

typedef struct _Group {
	/* Linked list of vertices in group */
	Node* vertices;

	/* Adjacency matrix and B matrix sums */
	spmat* A_g;
	double* row_sums;

	struct _Group *next;
}Group;

/* Allocating group */
Group* allocate_group(int n_g);

/* Free all group resources */
void free_group(Group* group);

/* Print group */
void print_group(Group* group);

/* Push group curr_group to the top of a list heaf_ref pointing on*/
void push_group(Group** head_ref, Group* curr_group);

/* Pop group from a list head_ref pointing on */
Group* pop_group(Group** head_ref);

/* Frees a group list */
void free_group_list(Group* head_ref, int n);

/* Calculates group list length */
int get_group_length(Group* group, int n);

#endif /* GROUP_H_ */
