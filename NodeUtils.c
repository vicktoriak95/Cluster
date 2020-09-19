/*
 * NodeUtils.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "NodeUtils.h"
#include "LibFuncsHandler.h"

Node* create_node(int x){
	Node* node = NULL;

	node = (Node*)allocate(sizeof(Node));
	node->index = x;
	node->next = NULL;
	return node;
}

void push_node(Node** head_ref, Node* new_node){
    /* Make next of new node as head */
    new_node->next = (*head_ref);
    /* Move the head to point to the new node */
    (*head_ref) = new_node;
}

void delete_node_list(Node* head_ref, int n){
	int cnt = 0;
	Node* current = head_ref;
	Node* next = NULL;

	while (current != NULL){
		infinite_loop_detection(cnt, n);
		next = current->next;
		free(current);
		current = next;
		cnt += 1;
	}
}

void print_node_list(Node* list){
	if (list == NULL){
		printf(" \n");
	}
	else {
		printf("%d ", list->index);
		print_node_list((Node*)(list->next));
	}
}

int get_node_value(Node* g, int k){
	Node* head = g;
	int i = 0;

	for (i = 0; i < k; i++){
		head = head->next;
	}
	return head->index;
}

int get_node_length(Node* g, int n){
	Node* head = g;
	int length = 0;
	int cnt = 0;

	while (head != NULL){
		infinite_loop_detection(cnt, n);
		length += 1;
		head = head->next;
		cnt += 1;
	}
	return length;
}

Node* node_list_from_vector(int* v, int n){
	Node* head = NULL;
	Node* node = NULL;
	int i = 0;

	for (i = n-1; i >= 0; i--){
		node = create_node(v[i]);
		push_node(&head, node);
	}
	return head;

}

void vector_from_list(int* vector, Node* g ,int n){
	int i = 0;
	Node* g_p = g;

	for(i = 0; i < n; i++){
		vector[i] = g_p->index;
		g_p = g_p->next;
	}
}

Node* divide_node_list(Node** g1_p, double* s, int n_g){
       Node* g2 = NULL;
       Node* g1_head = *g1_p;
       Node* g1_head_prev = NULL;
       Node* g2_head = g2;
       Node* to_be_g2_head = NULL;
       int i = 0;

       /* Iterating over s */
       for (i = 0; i < n_g; i ++){
               /* If we do not move the node from g1 */
               if (s[i] > 0){
                       g1_head_prev = g1_head;
                       g1_head = g1_head->next;
               }
               /* If we move the node to g2 */
               else {
                       to_be_g2_head = g1_head;
                       /* If we are moving first node from g1 */
                       if (g1_head_prev == NULL) {
                               *g1_p = g1_head->next;
                               g1_head = g1_head->next;
                       }
                       else {
                               g1_head_prev->next = g1_head->next;
                               g1_head = g1_head_prev->next;
                       }
                       /* If we are moving first node to g_2 */
                       if (g2_head == NULL){
                               g2 = to_be_g2_head;
                               g2_head = to_be_g2_head;
                       }
                       else {
                               g2_head->next = to_be_g2_head;
                               g2_head = g2_head->next;
                       }
                       g2_head->next = NULL;
               }
       }
       return g2;
}

Old_Group* create_group(Node* node){
	Old_Group* group = NULL;

	group = (Old_Group*)allocate(sizeof(Old_Group));
	group->vertices = node;
	group->next = NULL;
	return group;
}

void push_group(Old_Group** head_ref, Node* new_node){
	/* Create new List */
	Old_Group* curr_group = create_group(new_node);

	if ((*head_ref) == NULL) {
		(*head_ref) = curr_group;
	}
	else {
		/* Make next of new list as head */
		curr_group->next = (*head_ref);
		/* Move the head to point to the new list*/
		(*head_ref) = curr_group;
	}
}

void delete_group(Old_Group* head_ref, int n){
	int cnt = 0;
	Old_Group* current = head_ref;
	Old_Group* next = NULL;

	while (current != NULL){
		infinite_loop_detection(cnt, n);
		next = current->next;
		delete_node_list(current->vertices, n);
		free(current);
		current = next;
		cnt += 1;
	}
}
/*
void print_group(Old_Group* group){
	if (group == NULL){
		printf("NULL\n");
	}
	else {
		print_node_list(group->vertices);
		printf("NULL\n");
		print_node_list((Node*)(group->next));
	}
}
*/

int get_group_length(Old_Group* group, int n){
	Old_Group* head = group;
	int length = 0;
	int cnt = 0;

	while (head != NULL){
		infinite_loop_detection(cnt, n);
		length += 1;
		head = head->next;
		cnt += 1;
	}
	return length;
}
