#include <stdlib.h>
#include <stdio.h>
#include "NodeUtils.h"

Node* create_node(int x){
	Node* node;
	node = (Node*)malloc(sizeof(node));
	if (node == NULL){
		printf("Memory allocation failed");
		exit(1);
	}
	node->index = x;
	node->next = NULL;
	return node;
}

void push_node(Node** head_ref, Node* new_node)
{
    /* Make next of new node as head */
    new_node->next = (*head_ref);
    /* move the head to point to the new node */
    (*head_ref) = new_node;
}

/* Receives the head of the list and the number of nodes in the entire graph*/
void delete_node_list(Node** head_ref, int n){
	int cnt = 0;
	Node* current = *head_ref;
	Node* next;

	while ((cnt < n) && (current != NULL)){
		if (cnt >= n){
			printf("Infinite loop - delete_node_list");
			exit(1);
		}
		next = current->next;
		free(current);
		current = next;
		cnt++;
	}
}

void print_node_list(Node* list){
	if(list == NULL)
	{
		printf("NULL\n");
	}
	else
	{
		printf("%d", list->index);
		print_node_list((Node*)(list->next));
	}
}

Group* create_group(Node* node){
	Group* group;
	group = (Group*)malloc(sizeof(Group));
	if (group == NULL){
		printf("Memory allocation failed - create_group");
		exit(1);
	}
	group->value = node;
	group->next = NULL;
	return group;
}

void push_group(Group** head_ref, Node* new_node){
	/* Create new List */
	Group* curr_group = create_group(new_node);
	/* Make next of new list as head */
	curr_group->next = (*head_ref);
	/* Move the head to point to the new list*/
	(*head_ref) = curr_group;
}

void delete_group(Group** head_ref, int n){
	int cnt = 0;
	Group* current = *head_ref;
	Group* next;

	while ((cnt < n) && (current != NULL)){
		if (cnt >= n){
			printf("Infinite loop - delete_node_list");
			exit(1);
		}
		next = current->next;
		delete_node_list(&(current->value), n);
		free(current);
		current = next;
		cnt++;
		}
}

void print_group(Group* group){

	if(group == NULL){
		printf("NULL\n");
	}
	else
	{
		print_node_list(group->value);
		printf("NULL\n");
		print_node_list((Node*)(group->next));
	}
}

