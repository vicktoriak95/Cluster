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

void spmat_mult(const spmat* A, const double *v, double *result, Node* g){
	int i = 0;
	int g_index = 0;
	int mat_index = 0;
	int cnt = 0;
	int v_index = 0;
	int result_index = 0;
	double dot_product = 0;
	Node_matrix* mat_col = NULL;
	Node* g_rows = g;
	Node* g_cols = g;

	/* Iterating over A rows */
	for (i = 0; i < A->n; i++){
		/* If row is in g */
		if ((g_rows != NULL) && (i == g_rows->index)){
			dot_product = 0;
			mat_col = ((Node_matrix** )A->private)[i];
			g_cols = g;
			cnt = 0;
			v_index = 0;

			/* Iterating over list of a row, multiplying only items in g*/
			while ((mat_col != NULL) && (g_cols != NULL)){
				/* Infinite loop Detection */
				infinite_loop_detection(cnt, A->n);

				/* Promoting pointers in g and in row */
				g_index = g_cols->index;
				mat_index = mat_col->col_index;
				if (mat_index == g_index){
					dot_product += (mat_col->value) * (v[v_index]);
					g_cols = g_cols->next;
					v_index += 1;
					mat_col = (Node_matrix*)mat_col->next;
				}
				else if (mat_index > g_index){
					g_cols = g_cols->next;
					v_index += 1;
				}
				else {
					mat_col = (Node_matrix*)mat_col->next;
				}
				cnt += 1;
			}
			/* Updating result vector */
			result[result_index] = dot_product;
			result_index += 1;
			g_rows = g_rows->next;
		}

	}
}

int spmat_row_sum(spmat* A, int row_num, Node* g){
	Node_matrix* row_head = NULL;
	Node* g_head = g;
	int sum = 0;
	int cnt = 0;
	int mat_row_index = 0;
	int mat_col_index = 0;
	int g_col_index = 0;

	/* Find row index in A*/
	mat_row_index = get_node_value(g, row_num);

	row_head = ((Node_matrix** )A->private)[mat_row_index];

	/* Iterating over row, summing only entries in g */
	while ((row_head != NULL) && (g_head != NULL)){
		infinite_loop_detection(cnt, A->n);

		/* Comparing indices and promoting g_head, mat_head respectively */
		mat_col_index = row_head->col_index;
		g_col_index = g_head->index;
		if (mat_col_index == g_col_index){
			sum += row_head->value;
			g_head = g_head->next;
			row_head = (Node_matrix*)row_head->next;
		}
		else if (mat_col_index > g_col_index){
			g_head = g_head->next;
		}
		else {
			row_head = (Node_matrix*)row_head->next;
		}
		cnt += 1;
	}
	return sum;
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
	int A1_n = 0;
	int A2_n = 0;
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


	/* Calc the size of the new spmats */
	A1_n = 0;
	for (i=0; i<A->n; i++){
		if (s[i] == 1) {
			A1_n += 1;
		}
	}
	A2_n = A->n - A1_n;

	/* Allocate the new spmats */
	(*A1) = spmat_allocate(A1_n);
	(*A2) = spmat_allocate(A2_n);

	/* Calc the new_col_index. */
	new_col_index = (double*)allocate(A->n);
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

	/* Free the old spmat and the new_col_index*/
	free(new_col_index);
	spmat_free(A);

}
