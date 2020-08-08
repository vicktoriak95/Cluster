/*
 * spmat.h
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */
#include "NodeUtils.h"

#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_

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

/* Adds row i the matrix. Called before any other call, */
/* exactly n times in order (i = 0 to n-1) */
void spmat_add_row(struct _spmat *A, const double *row, int i);

/* Frees all resources used by A */
void spmat_free(spmat *A);

/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
void spmat_mult(const struct _spmat *A, const double *v, double *result, Node* g, int n_g);

#endif /* SPARSEMATRIX_H_ */
