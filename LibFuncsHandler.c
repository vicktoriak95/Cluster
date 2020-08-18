/*
 * LibFuncsHandler.c
 *
 *  Created on: 8 ����� 2020
 *      Author: user
 */

#include <stdlib.h>
#include <stdio.h>

FILE* open_file(char *pathname, const char *mode){
	FILE* f = NULL;

	f = fopen(pathname, mode);
	if (f == NULL){
		printf("File opening failed");
		exit(1);
	}
	return f;
}

int close_file(FILE *stream){
	int result = 0;

	result = fclose(stream);
	if (result == EOF){
		printf("File closing failed");
		exit(1);
	}
	return result;
}

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

void int_fwrite(int n, FILE* f){
	int return_value = 0;

	/* write n to the file */
	return_value = fwrite(&n, sizeof(int), 1, f);
	if (return_value != 1) {
		printf("File writing failed");
		exit(1);
	}
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

void infinite_loop_detection(int counter, int max_counter){
	if(counter >= max_counter){
		printf("Infinite loop error");
		exit(1);
	}
}
