/*
 * cluster.h
 *
 * Definition of node Struct which represents vertex in network.
 *
 * Functions: classic Node functions such as create node, push node, delete list, etc.
 *
 */

#ifndef NODEUTILS_H_
#define NODEUTILS_H_

typedef struct _Node {
	int index;
	struct _Node *next;

}Node;

/* Creates new node(=vertex) with index x */
Node* create_node(int x);

/* Insert a new node to the beginning of list */
void push_node(Node** head_ref, Node* new_node);

/* Delete the entire list.
 * Receives the head of the list and the number of nodes in the entire graph. */
void delete_node_list(Node* head_ref, int n);

/* Printing list */
void print_node_list(Node* list);

/* Iterating over node list g and count the length of the list */
int get_node_list_length(Node* g, int n);

/* Creating node list out of vector */
Node* node_list_from_vector(int* v, int n);

/* Receives allocated vector "vector" and list g,
 * updating vector with values of list g. */
void vector_from_list(int* vector, Node* g ,int n);

/* Dividing g according to s.
 * Editing group g into group g1 ,and returns group g2 */
Node* divide_node_list(Node** g1_p, double* s, int n_g);

#endif /* NODEUTILS_H_ */
