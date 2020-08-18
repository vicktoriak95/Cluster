/*
 * cluster.h
 *
 *  Created on: Aug 6, 2020
 *      Author: adi
 */


#ifndef NODEUTILS_H_
#define NODEUTILS_H_

typedef struct _Node {
	int index;
	struct _Node *next;

}Node;

/* Creates new node with index x */
Node* create_node(int x);

/* Insert a new node to the beginning of list */
void push_node(Node** head_ref, Node* new_node);

/* Delete the entire list */
void delete_node_list(Node** head_ref, int n);

/* Printing list */
void print_node_list(Node* list);

/* Iterating over g and returns k'th value */
int get_node_value(Node* g, int k);

/* Iterating over g and count the length of the list */
int get_node_length(Node* g);

/* Creating node list out of vector */
Node* node_list_from_vector(int* v, int n);

/* Given a list and its size, copies the list to given address */
void copy_node_list(Node* original, Node* copy);

void vector_from_list(int* vector, Node* g ,int n);


/*list of lists*/
typedef struct _LinkedGroup {
	Node* value;
	struct _LinkedGroup *next;

}Group;

/* Creates list representing group */
Group* create_group(Node* node);

/* Inserts new list to the g */
void push_group(Group** head_ref, Node* new_node);

/* Deletes group */
void delete_group(Group** head_ref, int n);

/* Prints group */
void print_group(Group* group);

/* Iterating over the group list and count the length of the list */
int get_group_length(Group* group);


#endif /* NODEUTILS_H_ */
