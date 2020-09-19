/*
 * spmat.h
 *
 * Definition of spmat struct represents sparse matrix.
 * Definition of Node_matrix struct, because we chose to implement the linked list version of sparse matrix.
 *
 * Functions: all functions which handles sparse matrix: allocation and free, reading row from file and vector,
 * multiplication by vector and row sum.
 *
 */

#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_
#define ENTRY_VAL  1

#include "NodeUtils.h"
#include <stdio.h>

typedef struct _spmat {
	/* Matrix size (n*n) */
	int		n;

	/* Private field for inner implementation.
	 * Should not be read or modified externally */
	void	*private;

} spmat;

/* Definition of node struct representing entries in sparse matrix */
typedef struct _Node_matrix {
    int value;
    int col_index;
    struct _Node_matrix* next;
} Node_matrix;

/* Create node in a list representing row */
Node_matrix* nodemat_create_node(int value, int col_index);

/* Free list of a row */
void nodemat_free_list(Node_matrix* list);

/* Allocates a new linked-lists sparse matrix of size n */
spmat* spmat_allocate(int n);

/* Add a single row from vector */
void spmat_add_row_from_vector(spmat* A, const int *row, int i);

/* Read and add a single row from file*/
/* Returns number of non-zero entries in row - deg of vertex the row representing */
int spmat_add_row_from_file(spmat* A ,FILE* input, int i);

/* Free all resources used by A.
 * Frees lists of all rows, array of rows and spmat itself */
void spmat_free(spmat *A);

/* Multiplies sub-matrix matrix A[g] by vector v, into result (result is pre-allocated) */
void spmat_mult(const spmat* A, const double *v, double *result, Node* g);

/* Sums row values of sub-matrix A[g], according to g values */
int spmat_row_sum(spmat* A, int row_num, Node* g);

/* Prints sparse matrix */
void print_sparse_matrix(spmat* sp_mat);

/* Prints node matrix list */
void print_node_matrix_list(spmat* sp_mat, int i);

void divide_spmat(spmat* A, double* s, spmat** A1, spmat** A2, int A1_n, int A2_n);

#endif /* SPARSEMATRIX_H_ */
