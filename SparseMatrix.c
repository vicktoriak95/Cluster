/*
 * SparseMatrix.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "SparseMatrix.h"
#include "LibFuncsHandler.h"

Node_matrix* nodemat_create_node(int value, int col_index){
	Node_matrix* node = NULL;

	node = (Node_matrix*)allocate(sizeof(Node_matrix));
	node->value = value;
	node->col_index = col_index;
	node->next = NULL;

	return node;
}

void nodemat_free_list(Node_matrix* list){
	if (list != NULL){
		nodemat_free_list((Node_matrix*)list->next);
		free(list);
	}
}

spmat* spmat_allocate(int n){
	spmat* A = NULL;
	Node_matrix** private = NULL;
	int i = 0;

	/* Allocating mem for A */
	A = (spmat*)allocate(sizeof(spmat));

	/* Using calloc to assure all pointers are NULL */
	private = (Node_matrix**)allocate(n * sizeof(Node_matrix**));
	for(i = 0; i < n; i++){
		private[i] = NULL;
	}

	A->n = n;
	A->private = private;

	return A;
}

void spmat_add_row_from_vector(spmat* A, const int *row, int i){
	/* Tail Pointing on last node added */
	Node_matrix* tail = NULL;
	/* Node pointing on current added node */
	Node_matrix* node = NULL;
	int j = 0;

	/* Iterating over row values */
	for (j = 0; j < A->n; j++){
		if (row[j] != 0){
			/* Creating node for non zero value */
			node = nodemat_create_node(row[j], j);

			/* List is empty - first node appended */
			if (tail == NULL){
				((Node_matrix** )A->private)[i] = node;
			}
			/* List is not empty */
			else {
				tail->next = node;
			}
			tail = node;
		}
	}
}

int spmat_add_row_from_file(spmat* A ,FILE* input, int i){
	int j = 0;
	int nnz_row = 0;
	int col_index = 0;
	Node_matrix* node = NULL;
	Node_matrix* tail = NULL;

	/* Reading degree of vertice i = num of non zero values in row i */
	nnz_row = int_fread(input);
	for (j = 0; j < nnz_row; j++){
		col_index = int_fread(input);
		/* Node is always pointing on current added node.
		 * ENTRY_VAL is const and equals 1. */
		node = nodemat_create_node(ENTRY_VAL, col_index);

		/* Inserting node to spmat.
		 * List is empty - first node appended */
		if (tail == NULL){
			((Node_matrix** )A->private)[i] = node;
		}
		/* List is not empty */
		else {
			tail->next = node;
		}
		/* Tail always Pointing on last node added */
		tail = node;
	}
	return nnz_row;
}

void spmat_free(spmat* A){
	int i = 0;

	for (i = 0; i < A->n; i++){
		nodemat_free_list(((Node_matrix** )A->private)[i]);
	}
	free(A->private);
	free(A);
}

void spmat_mult(const spmat* A, const double *v, double *result){
	int i = 0;
	Node_matrix* node = NULL;
	double res = 0;
	int c = 0;

	/* Iterate over A rows */
	for (i=0; i<A->n; i++){
		node = ((Node_matrix**)(A->private))[i];
		res = 0;
		/* Iterate over all nnz entries in A[i] */
		c = 0;
		while (node != NULL){
			infinite_loop_detection(c, A->n);
			res += node->value * v[node->col_index];
			node = node->next;
			c += 1;
		}
		result[i] = res;
	}
}

void print_sparse_matrix(spmat* sp_mat){
	int i = 0;

	/* Iterating over spmat private i.e. over rows*/
	for (i = 0; i < sp_mat->n; i++){
		print_node_matrix_list(sp_mat, i);
		printf("\n");
	}
}

void print_node_matrix_list(spmat* sp_mat, int i){
	int j = 0;
	Node_matrix* sparse_node = NULL;

	/* Getting the list that represents row i*/
	sparse_node = ((Node_matrix**)sp_mat->private)[i];

	for (j = 0; j < sp_mat->n; j++){
		if ((sparse_node != NULL) && (sparse_node->col_index == j)){
			printf("1 ");
			sparse_node = sparse_node->next;
		}
		else{
			printf("0 ");
		}
	}
}

void divide_spmat(spmat* A, double* s, spmat** A1, spmat** A2){
	int i = 0;
	int old_row = 0;
	int old_col = 0;
	int new_col = 0;
	int new_row = 0;
	double* new_col_index = NULL;
	int A1_col = 0;
	int A2_col = 0;
	Node_matrix* old_node = NULL;
	Node_matrix* new_node = NULL;
	Node_matrix* tail = NULL;

	/* Calc the new_col_index. */
	new_col_index = (double*)allocate(sizeof(double) * A->n);
	A1_col = 0;
	A2_col = 0;
	for (i=0; i<A->n; i++){
		if (s[i] == 1){
			new_col_index[i] = A1_col;
			A1_col += 1;
		}
		else {
			new_col_index[i] = A2_col;
			A2_col += 1;
		}
	}

	/* Iterate over the spmat and fill the new spmats */
	/* Iterate over the spmat rows */
	for (old_row=0; old_row<A->n; old_row++){
		/* head points to the last node in the row of the new spmat we are currently filling */
		new_row = new_col_index[old_row];
		if (s[old_row] == 1){
			tail = ((Node_matrix**)((*A1)->private))[new_row];
		}
		else {
			tail = ((Node_matrix**)((*A2)->private))[new_row];
		}

		old_node = ((Node_matrix**)(A->private))[old_row];
		/* Iterate over the spmat cols */
		while (old_node != NULL){
			old_col = old_node->col_index;
			/* If the old_node represents an edge inside one of the new spmats */
			if (s[old_row] == s[old_col]){
				new_col = new_col_index[old_col];
				new_node =  nodemat_create_node(ENTRY_VAL, new_col);
				if (tail == NULL){
					if (s[old_row] == 1){
						((Node_matrix**)((*A1)->private))[new_row] = new_node;
					}
					else {
						((Node_matrix**)((*A2)->private))[new_row] = new_node;
					}
				}
				else {
					tail->next = new_node;
				}
				tail = new_node;
			}
			old_node = old_node->next;
		}
	}

	free(new_col_index);
}
