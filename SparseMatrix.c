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
	node = (Node_matrix *)malloc(sizeof(Node_matrix));
	if(node == NULL){
		printf("Memory allocation failed - list_create_node");
		exit(1);
	}
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
	Node** private;

	/* Allocating mem for A */
	A = (spmat*)malloc(sizeof(spmat));
	if(A == NULL){
		printf("Memory allocation failed - spmat_allocate_list 1");
		exit(1);
	}

	/* Using calloc to assure all pointers are NULL */
	private = (Node**)calloc(n, sizeof(Node*));
	if(private == NULL){
		printf("Memory allocation failed - spmat_allocate_list 1");
	}

	A->n = n;
	A->private = private;

	return A;
}

/* TODO: erase if only used in tests */
/* Add a single row from vector*/
void spmat_add_row_from_vector(struct _spmat *A, const int *row, int i){
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
				tail->next = (struct Node_matrix*)node;
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
			tail->next = (struct Node_matrix*)node;
		}
		/* Tail always Pointing on last node added */
		tail = node;
	}
	return nnz_row;
}

/* Free lists of all rows, array of rows and spmat itself */
void spmat_free(struct _spmat *A){
	int i;

	for(i = 0; i< A->n; i++){
		nodemat_free_list(((Node_matrix** )A->private)[i]);
	}
	free(A->private);
	free(A);
}


/* Multiplies matrix A by vector v, into result (result is pre-allocated) */
void spmat_mult(const struct _spmat *A, const double *v, double *result, Node* g, int n_g){
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
int spmat_row_sum(struct _spmat *A, int row_num, Node* g, int n_g){
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

/* Creates spmat from matrix */
/* TODO: DEBUG CAUSE NOT WORKING */
spmat* spmat_from_matrix(int** matrix, int n){
	spmat* A;
	int i;

	A = spmat_allocate(n);
	for(i = 0; i < n; i++){
		spmat_add_row_from_vector(A, matrix[i], i);
	}
	printf("created A \n");
	return A;
}

/* TODO: erase */
void test_sparse(){
	spmat* A;
	Node* g;
	Node* node;
	int row_sum;
	int matrix[4][4] = {{0,0,0,0},{1,300,8,500},{0,0,0,0},{4,1,2,0}};
	int i;
	double v[2] = {1,1};
	double result[2];
	int row_num;
	int n;

	/* Creating g */
	g = create_node(3);
	node = create_node(1);
	push_node(&g, node);
	row_num = 2;
	printf("created g \n");

	/* Creating A */
	n = 4;
	A = spmat_allocate(n);
	for(i = 0; i < n; i++){
		spmat_add_row_from_vector(A, matrix[i], i);
	}
	printf("created A \n");

	/* Multiplying A by v */
	spmat_mult(A, v, result, g, row_num);
	printf("Mult result: \n");
	print_vector(result, row_num);

	/* Summing first row */
	/*
	row_sum = spmat_row_sum(A, 0, g, 2);
	printf("First row sum %d \n", row_sum);
	*/

	/* Summing all rows */
	for(i=0; i<row_num; i++){
		row_sum = spmat_row_sum(A, i, g, row_num);
		printf("%d row sum %d \n", i, row_sum);
	}

	free(A);

	printf("Vicki is the Best");
}

/* TODO: erase */
/*
int main(int argc, char* argv[]){
	test();

}
*/
