/*
 * NodeUtils.c
 *
 *  Created on: Aug 10, 2020
 *      Author: User
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
    /* move the head to point to the new node */
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

Group* create_group(Node* node){
	Group* group = NULL;

	group = (Group*)allocate(sizeof(Group));
	group->value = node;
	group->next = NULL;
	return group;
}

void push_group(Group** head_ref, Node* new_node){
	/* Create new List */
	Group* curr_group = create_group(new_node);

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

void delete_group(Group* head_ref, int n){
	int cnt = 0;
	Group* current = head_ref;
	Group* next = NULL;

	while (current != NULL){
		infinite_loop_detection(cnt, n);
		next = current->next;
		delete_node_list(current->value, n);
		free(current);
		current = next;
		cnt += 1;
	}
}

void print_group(Group* group){
	if (group == NULL){
		printf("NULL\n");
	}
	else {
		print_node_list(group->value);
		printf("NULL\n");
		print_node_list((Node*)(group->next));
	}
}

int get_group_length(Group* group, int n){
	Group* head = group;
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
