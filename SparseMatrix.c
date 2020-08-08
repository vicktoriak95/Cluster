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


/* Add a single row */
void spmat_add_row(spmat *A, const double *row, int i){
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

void spmat_add_row_from_file(spmat* A ,FILE* input, int i){
	int j;
	int nnz_row, col_index;
	Node_matrix* node = NULL;
	Node_matrix* tail = NULL;

	/* Reading degree of vertice i = num of non zero values in row i */
	nnz_row = int_fread(input);
	for(j=0; j<nnz_row; j++){
		col_index = int_fread(input);
		/* TODO: change 1 to const / change func */
		node = nodemat_create_node(1, col_index);

		/* Inserting node to spmat */
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
	int cnt = 0;

	/* Iterating over A rows */
	for(i = 0; i < A->n; i++){

		/* If row is in g */
		if((g_rows != NULL) && (i == g_rows->index)){
			dot_product = 0;
			head = ((Node_matrix** )A->private)[i];
			g_cols = g;

			/* Iterating over list of a row, multiplying only items in g*/
			while((head != NULL) && (g_cols != NULL)){

				/* Infinite loop Detection */
				if(cnt >= n_g){
					printf("Infinite loop - spmat_mult");
					exit(1);
				}
				if(head->col_index == g_cols->index){
					dot_product += (head->value) * (v[head->col_index]);
					g_cols = g_cols->next;
				}
				head = (Node_matrix*)head->next;
				cnt ++;
			}

			result[i] = dot_product;
			g_rows = g_rows->next;
		}

	}
}


/* Sums row values of A, according to g values */
int spmat_row_sum(struct _spmat *A, int row_num, Node* g, int n_g){
	Node_matrix* row_head = ((Node_matrix** )A->private)[row_num];
	Node* g_head = g;
	int sum = 0;
	int cnt = 0;

	/* Iterating over row, summing only entries in g */
	while((row_head != NULL) && (g_head != NULL)){
		/* Infinite loop Detection */
		if(cnt >= n_g){
			printf("Infinite loop - spmat_row_sum");
			exit(1);
		}
		if(row_head->col_index == g_head->index){
			sum += row_head->value;
		}
	}

	return sum;
}

void test(){
spmat* A;
	Node* g;
	Node* node;
	int row_sum;
	double matrix[4][4] = {{0,0,0,0},{0,0,1,5},{0,0,0,0},{4,0,2,0}};
	int i;
	double v[4] = {1,1,1,1};
	double result[4];

	/* Creating g */
	g = create_node(3);
	node = create_node(1);
	push_node(&g, node);
	printf("created g \n");

	/* Creating A */
	A = spmat_allocate(4);
	for(i = 0; i < 4; i++){
		spmat_add_row(A, matrix[i], i);
	}
	printf("created A \n");

	/* Multiplying A by v */
	spmat_mult(A, v, result, g, 2);
	printf("Mult result: \n");
	print_vector(v, 4);

	/* Summing first row */
	row_sum = spmat_row_sum(A, 0, g, 2);
	printf("First row sum %d", row_sum);
	free(A);

	printf("Vicki is the Best");
}

int main(int argc, char* argv[]){
	test();

}
