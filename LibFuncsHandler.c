/*
 * LibFuncsHandler.c
 *
 *  Created on: 8 баев„ 2020
 *      Author: user
 */

#include <stdlib.h>
#include <stdio.h>

/* Read an integer from an open file descriptor */
int int_fread(FILE* f){
	int n = 0;
	int return_value = 0;

	/* read int from the file */
	return_value = fread(&n, sizeof(int), 1, f);
	if (return_value != 1) {
		printf("File reading failed");
		exit(1);
	}

	return n;
}

/* Write an integer to an open file descriptor */
void int_fwrite(int n, FILE* f){
	int return_value = 0;

	/* write n to the file */
	return_value = fwrite(&n, sizeof(int), 1, f);
	if (return_value != 1) {
		printf("File writing failed");
		exit(1);
	}
}

/* Allocate n bytes and return a pointer to the allocated space */
void* allocate(size_t n){
	void* p = NULL;
	p = malloc(n);
	if (p == NULL){
		printf("Memory allocation failed");
		exit(1);
	}
	return p;
}

/* Detects infinite loops and exits */
void infinite_loop_detection(int counter, int max_counter){
	if(counter >= max_counter){
		printf("Infinite loop error");
		exit(1);
	}
}
