/*
 * LibFuncsHandler.c
 *
 *  Created on: 8 баев„ 2020
 *      Author: user
 */

#include <stdlib.h>
#include <stdio.h>

FILE* open_file(char *pathname, const char *mode){
	FILE* f = NULL;

	f = fopen(pathname, mode);
	if (f == NULL){
		printf("File opening failed\n");
		exit(1);
	}
	return f;
}

int close_file(FILE *stream){
	int result = 0;

	result = fclose(stream);
	if (result == EOF){
		printf("File closing failed\n");
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
		printf("File reading failed\n");
		exit(1);
	}
	return n;
}

void int_fwrite(int n, FILE* f){
	int return_value = 0;

	/* write n to the file */
	return_value = fwrite(&n, sizeof(int), 1, f);
	if (return_value != 1) {
		printf("File writing failed\n");
		exit(1);
	}
}

void* allocate(size_t n){
	void* p = NULL;
	p = malloc(n);
	if (p == NULL){
		printf("Memory allocation failed\n");
		exit(1);
	}
	return p;
}

void infinite_loop_detection(int counter, int max_counter){
	if(counter >= max_counter){
		printf("Infinite loop error\n");
		exit(1);
	}
}

void assert_file_not_empty(FILE* f){
	int size = 0;
	int result = 0;

    result = fseek (f, 0, SEEK_END);
    if (result != 0){
    	printf("File seek failed\n");
    	exit(1);
    }

    size = ftell(f);
    if (size == -1){
    	printf("File tell failed\n");
    	exit(1);
    }

    if (size == 0) {
        printf("File is empty\n");
        exit(1);
    }
}
