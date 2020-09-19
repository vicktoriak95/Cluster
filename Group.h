/*
 * Group.h
 *
 * TODO: add documentation
 *
 */

#ifndef GROUP_H_
#define GROUP_H_
#include "SparseMatrix.h"
#include "NodeUtils.h"

/* TODO */
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

/* Free all group resorces */
void free_group(Group* group);

void print_group(Group* group);

#endif /* GROUP_H_ */
