/*
 * spmat.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_
#define entry_val  1

#include "NodeUtils.h"

typedef struct _spmat {
	/* Matrix size (n*n) */
	int		n;

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	void	*private;

} spmat;

/* Definition of node struct representing entries in sparse matrix*/
typedef struct _Node_matrix {
    double value;
    int col_index;
    struct _Node_matrix* next;
} Node_matrix;

/* Create node in a list representing row */
Node_matrix* nodemat_create_node(int value, int col_index);

/* Free list of a row */
void nodemat_free_list(Node_matrix* list);

/* Allocates a new linked-lists sparse matrix of size n */
spmat* spmat_allocate(int n);

/* Add a single row from vector*/
void spmat_add_row_from_vector(spmat* A, const int *row, int i);

/* Adds row i the matrix from file*/
/* Returns num of non-zero entries in row */
int spmat_add_row_from_file(spmat* A ,FILE* input, int i);

/* Frees all resources used by A */
/* Free lists of all rows, array of rows and spmat itself */
void spmat_free(spmat *A);

/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
void spmat_mult(const spmat* A, const double *v, double *result, Node* g);

/* TODO: make sure is fine with double */
/* Sums row values of A, according to g values */
int spmat_row_sum(spmat* A, int row_num, Node* g);

void print_sparse_matrix(spmat* sp_mat);

void print_node_matrix_list(spmat* sp_mat, int i);

#endif /* SPARSEMATRIX_H_ */
