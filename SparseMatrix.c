/*
 * SparseMatrix.c
 *
 *  Created on: Aug 8, 2020
 *      Author: User
 */

#include <stdio.h>
#include <stdlib.h>
#include "SparseMatrix.h"
#include "NodeUtils.h"
#include "LinearUtils.h"
#include "LibFuncsHandler.h"


/* Create node in a list representing row */
Node_matrix* nodemat_create_node(int value, int col_index){
	Node_matrix* node;

	node = (Node_matrix*)allocate(sizeof(Node_matrix));
	node->value = value;
	node->col_index = col_index;
	node->next = NULL;

	return node;
}


/* Free list of a row */
void nodemat_free_list(Node_matrix* list){
	if (list != NULL){
		nodemat_free_list((Node_matrix*)list->next);
		free(list);
	}
}


/* Allocates a new linked-lists sparse matrix of size n */
spmat* spmat_allocate(int n){
	spmat* A;
	Node_matrix** private;

	/* Allocating mem for A */
	A = (spmat*)allocate(sizeof(spmat));

	/* Using calloc to assure all pointers are NULL */
	private = (Node_matrix**)allocate(n * sizeof(Node_matrix**));

	A->n = n;
	A->private = private;

	return A;
}

/* Add a single row from vector*/
void spmat_add_row_from_vector(spmat* A, const int *row, int i){
	/* Tail Pointing on last node added */
	Node_matrix* tail = NULL;
	/* Node pointing on current added node */
	Node_matrix* node = NULL;
	int j;

	/* Iterating over row values */
	for(j = 0; j < A->n; j++){
		if(row[j] != 0){
			/* Creating node for non zero value */
			node = nodemat_create_node(row[j], j);

			/* List is empty - first node appended */
			if(tail == NULL){
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

/* Adds row i the matrix from file*/
/* Returns num of non-zero entries in row*/
int spmat_add_row_from_file(spmat* A ,FILE* input, int i){
	int j;
	int nnz_row, col_index;
	Node_matrix* node = NULL;
	Node_matrix* tail = NULL;

	/* Reading degree of vertice i = num of non zero values in row i */
	nnz_row = int_fread(input);
	for(j=0; j<nnz_row; j++){
		col_index = int_fread(input);
		/* Node is always pointing on current added node */
		/* entry_val is const and equals 1 */
		node = nodemat_create_node(entry_val, col_index);

		/* Inserting node to spmat */
		/* List is empty - first node appended */
		if(tail == NULL){
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

/* Free lists of all rows, array of rows and spmat itself */
void spmat_free(spmat* A){
	int i;

	for(i = 0; i< A->n; i++){
		nodemat_free_list(((Node_matrix** )A->private)[i]);
	}
	free(A->private);
	free(A);
}


/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
void spmat_mult(const spmat* A, const double *v, double *result, Node* g){
	int i, g_index, mat_index;
	int cnt = 0;
	int v_index = 0;
	int result_index = 0;
	double dot_product;
	Node_matrix* mat_col = NULL;
	Node* g_rows = g;
	Node* g_cols = g;

	/* Iterating over A rows */
	for(i = 0; i < A->n; i++){
		/* If row is in g */
		if((g_rows != NULL) && (i == g_rows->index)){
			dot_product = 0;
			mat_col = ((Node_matrix** )A->private)[i];
			g_cols = g;
			cnt = 0;
			v_index = 0;

			/* Iterating over list of a row, multiplying only items in g*/
			while((mat_col != NULL) && (g_cols != NULL)){
				/* Infinite loop Detection */
				infinite_loop_detection(cnt, A->n);

				/* Promoting pointers in g and in row */
				g_index = g_cols->index;
				mat_index = mat_col->col_index;
				if(mat_index == g_index){
					dot_product += (mat_col->value) * (v[v_index]);
					g_cols = g_cols->next;
					v_index += 1;
					mat_col = (Node_matrix*)mat_col->next;
				}
				else if(mat_index > g_index){
					g_cols = g_cols->next;
					v_index += 1;
				}
				else{
					mat_col = (Node_matrix*)mat_col->next;
				}
			}
			/* Updating result vector */
			result[result_index] = dot_product;
			result_index += 1;
			g_rows = g_rows->next;
		}

	}
}


/* Sums row values of A, according to g values */
int spmat_row_sum(spmat* A, int row_num, Node* g){
	Node_matrix* row_head;
	Node* g_head = g;
	int sum = 0;
	int cnt = 0;
	int mat_row_index, mat_col_index, g_col_index;

	/* find row index in A*/
	mat_row_index = get_node_value(g, row_num);

	row_head = ((Node_matrix** )A->private)[mat_row_index];

	/* Iterating over row, summing only entries in g */
	while((row_head != NULL) && (g_head != NULL)){
		infinite_loop_detection(cnt, A->n);

		/* Comparing indices and promoting g_head, mat_head respectively */
		mat_col_index = row_head->col_index;
		g_col_index = g_head->index;
		if(mat_col_index == g_col_index){
			sum += row_head->value;
			g_head = g_head->next;
			row_head = (Node_matrix*)row_head->next;
		}
		else if(mat_col_index > g_col_index){
			g_head = g_head->next;
		}
		else{
			row_head = (Node_matrix*)row_head->next;
		}
	}
	return sum;
}

void print_sparse_matrix(spmat* sp_mat){
	int i;

	/* Iterating over spmat private  i.e. over rows*/
	for(i=0; i < sp_mat->n; i++){
		print_node_matrix_list(sp_mat, i);
		printf("\n");
	}
}


void print_node_matrix_list(spmat* sp_mat, int i){

	int j;
	Node_matrix* sparse_node;

	/* Getting the list that represents row i*/
	sparse_node = ((Node_matrix**)sp_mat->private)[i];

	for(j = 0; j < sp_mat->n; j++){
		if((sparse_node != NULL) && (sparse_node->col_index == j)){
			printf("1 ");
			sparse_node = sparse_node->next;
		}
		else{
			printf("0 ");
		}
	}

}
