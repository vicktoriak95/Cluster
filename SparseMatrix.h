/*
 * spmat.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */
#include "NodeUtils.h"

#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_
#define entry_val  1

typedef struct _spmat {
	/* Matrix size (n*n) */
	int		n;

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	void	*private;

} spmat;

/* Allocates a new linked-lists sparse matrix of size n */
spmat* spmat_allocate(int n);

/* TODO: make sure is fine with double */
/* Sums row values of A, according to g values */
int spmat_row_sum(struct _spmat *A, int row_num, Node* g, int n_g);

/* Add a single row from vector*/
void spmat_add_row_from_vector(struct _spmat *A, const int *row, int i);

/* Adds row i the matrix from file*/
/* Returns num of non-zero entries in row*/
int spmat_add_row_from_file(spmat* A ,FILE* input, int i);

/* Frees all resources used by A */
void spmat_free(spmat *A);

/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
void spmat_mult(const struct _spmat *A, const double *v, double *result, Node* g, int n_g);

/* Sums row values of A, according to g values */
int spmat_row_sum(struct _spmat *A, int row_num, Node* g, int n_g);

/* Definition of node struct representing entries in sparse matrix*/
typedef struct {
    double value;
    int col_index;
    struct Node_matrix* next;
} Node_matrix;

/* Create node in a list representing row */
Node_matrix* nodemat_create_node(int value, int col_index);

/* Free list of a row */
void nodemat_free_list(Node_matrix* list);

#endif /* SPARSEMATRIX_H_ */
