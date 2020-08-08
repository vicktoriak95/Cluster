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

/* Definition of node struct representing entries in sparse matrix*/
typedef struct {
    double value;
    int col_index;
    struct Node_matrix* next;
} Node_matrix;

/* Create node in a list representing row */
Node_matrix* nodemat_create_node(double value, int col_index){
	Node_matrix* node;
	node = (Node_matrix *)malloc(sizeof(Node_matrix));
	if(node != NULL){
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

spmat* spmat_allocate(int n){
	spmat* A;
	Node** private;

	/* Allocating mem for A */
	A = (spmat*)malloc(sizeof(spmat));
	if(A != NULL){
		printf("Memory allocation failed - spmat_allocate_list");
		exit(1);
	}

	/* Using calloc to assure all pointers are NULL */
	private = (Node**)calloc(n, sizeof(Node*));
	if(private != NULL){
		printf("Memory allocation failed - spmat_allocate_list");
	}

	A->n = n;
	A->private = private;

	return A;
}


/* Add a single row */
void spmat_add_row(struct _spmat *A, const double *row, int i){
	/* Tail Pointing on last node added */
	Node_matrix* tail = NULL;
	/* Node pointing on current added node */
	Node_matrix* node = NULL;
	int j;

	/* Iterating over A values */
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
	int i;
	double dot_product;
	Node_matrix* head = NULL;
	Node* g_rows = g;
	Node* g_cols = g;

	/* Iterating over A rows */
	for(i = 0; i < A->n; i++){

		/* If row is in g */
		if(i == g_rows->index){
			dot_product = 0;
			head = ((Node_matrix** )A->private)[i];
			g_cols = g;

			/* Iterating over list of a row, multiplying only items in g*/
			while((head != NULL) && (g_cols !=NULL)){
				if(head->col_index == g_cols->index){
					dot_product += (head->value) * (v[head->col_index]);
				}
				head = (Node_matrix*)head->next;
				g_cols = g_cols->next;
			}
			result[i] = dot_product;
			g_rows = g_rows->next;
		}

	}
}


/* Sums row values of A, according to g values */
/*
spmat_row_sum(struct _spmat *A, int row_num, Node* g, int n_g){
	Node_matrix* row_head = A->private[row_num];
	Node* g_head = g;

	while(row_head != NULL){
		if(row_head->col_index == g_head->index){

		}
	}
}
*/
