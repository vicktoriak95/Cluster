/*
 * cluster.h
 *
 * Definition of two node Structs needed:
 * - Node which represents vertex in network.
 * - Group which represents a cluster of vertices, value is node pointer pointing on first vertex in cluster.
 * Group is used in O and P groups in main division algorithm (divide_net_to_clusters).
 *
 * TODO: change description if needed
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

typedef struct _LinkedGroup {
	Node* vertices;
	struct _LinkedGroup *next;

}Old_Group;

/* Creates new node(=vertex) with index x */
Node* create_node(int x);

/* Insert a new node to the beginning of list */
void push_node(Node** head_ref, Node* new_node);

/* Delete the entire list.
 * Receives the head of the list and the number of nodes in the entire graph. */
void delete_node_list(Node* head_ref, int n);

/* Printing list */
void print_node_list(Node* list);

/* Iterating over node list g and returns k'th node vertices */
int get_node_value(Node* g, int k);

/* Iterating over node list g and count the length of the list */
int get_node_length(Node* g, int n);

/* Creating node list out of vector */
Node* node_list_from_vector(int* v, int n);

/* Receives allocated vector "vector" and list g,
 * updating vector with values of list g. */
void vector_from_list(int* vector, Node* g ,int n);

/* Dividing g according to s.
 * Editing group g into group g1 ,and returns group g2 */
Node* divide_node_list(Node** g1_p, double* s, int n_g);

/* Creates node list representing group */
Old_Group* create_group(Node* node);

/* Inserts new group into group list */
void push_group(Old_Group** head_ref, Node* new_node);

/* Deletes group */
void delete_group(Old_Group* head_ref, int n);

/* Prints group */
/*
void print_group(Old_Group* group);
*/

/* Iterating over the group list and count the length of the list */
int get_group_length(Old_Group* group, int n);

#endif /* NODEUTILS_H_ */
