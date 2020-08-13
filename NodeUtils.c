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

void push_node(Node** head_ref, Node* new_node){
    /* Make next of new node as head */
    new_node->next = (*head_ref);
    /* move the head to point to the new node */
    (*head_ref) = new_node;
}

void delete_node(Node **head_ref, int key){
	Node* temp = *head_ref, *prev;

	/* If head node holds the key to be deleted */
	if (temp != NULL && temp->index == key){
		*head_ref = temp->next;
		free(temp);
		return;
	}

	/* Search for the key to be deleted, keep track of the
	previous node */
	while (temp != NULL && temp->index != key){
		prev = temp;
		temp = temp->next;
	}

	/* If key was not present in linked list */
	if (temp == NULL) return;

	/* Remove the node from linked list */
	prev->next = temp->next;
	free(temp);
}

/* Receives the head of the list and the number of nodes in the entire graph*/
void delete_node_list(Node** head_ref, int n){
	int cnt = 0;
	Node* current = *head_ref;
	Node* next;

	while ((cnt < n) && (current != NULL)){
		/* TODO: check infinite loop in a different way */
		/*if (cnt >= n){
			printf("Infinite loop - delete_node_list");
			exit(1);
		}*/
		next = current->next;
		free(current);
		current = next;
		cnt++;
	}
}

void print_node_list(Node* list){
	if(list == NULL)
	{
		printf(" \n");
	}
	else
	{
		printf("%d ", list->index);
		print_node_list((Node*)(list->next));
	}
}

/* Iterating over g and returns k'th value */
int get_node_value(Node* g, int k){
	Node* head = g;
	int i;

	for(i=0; i<k; i++){
		head = head->next;
	}
	return head->index;
}

/* Iterating over g and count the length of the list */
int get_node_length(Node* g){
	Node* head = g;
	int length = 0;

	while (head->next != NULL){
		length += 1;
		head = head->next;
	}
	return length;
}

/* Creating node list out of vector */
Node* node_list_from_vector(int* v, int n){
	Node* head = NULL;
	Node* node;
	int i;

	for(i=n-1; i>=0; i--){
		node = create_node(v[i]);
		push_node(&head, node);
	}
	return head;

}

void copy_node_list(Node* original, Node* copy){
	Node* curr_orig;
	Node* curr_copy;
	Node* temp;

	/* validate input */
	if (original == NULL){
		printf("original vector is NULL - copy_node_list");
		return;
	}

	curr_copy = create_node(original->index);
	curr_orig = original->next;

	while(curr_orig != NULL){
		temp = create_node(curr_orig->index);
		curr_copy->next = temp;
		curr_copy = temp;
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

void delete_group(Group** head_ref, int n){
	int cnt = 0;
	Group* current = *head_ref;
	Group* next;

	while ((cnt < n) && (current != NULL)){
		/* TODO: check infinite loop in a different way */
		/*if (cnt >= n){
			printf("Infinite loop - delete_node_list");
			exit(1);
		}*/
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

/* Iterating over the group list and count the length of the list */
int get_group_length(Group* group){
	Group* head = group;
	int length = 0;

	while (head->next != NULL){
		length += 1;
		head = head->next;
	}
	return length;
}

void test_node_utils(){
	Node* g = NULL;
	int v[4] = {1,2,3,4};
	int n = 4;

	/* Test list from vector */
	g = node_list_from_vector(v, n);
	printf("g is made \n");
	print_node_list(g);

}

/*
int main(int argc, char* argv[]){
	test_node_utils();

}
*/


