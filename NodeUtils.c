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

int get_node_list_length(Node* g, int n){
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
