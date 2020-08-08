/*
 * LibFuncsHandler.c
 *
 *  Created on: 8 ����� 2020
 *      Author: user
 */

#include <stdlib.h>
#include <stdio.h>

int int_fread(FILE* f){
	int n = 0;
	int return_value = 0;

	/* read int from the file */
	return_value = fread(&n, sizeof(int), 1, f);
	if (return_value != sizeof(int)) {
		printf("File reading failed");
		exit(1);
	}

	return n;
}

void* allocate(size_t n){
	void* p = NULL;
	p = malloc(n);
	if (p == NULL){
		printf("Memory allocation failed");
		exit(1);
	}
	return p;
}
