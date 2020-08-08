/*
 * LinearUtils.c
 *
 *  Created on: Aug 8, 2020
 *      Author: adi
 */


#include <stdlib.h>
#include <stdio.h>
#include "NodeUtils.h"
#include "Network.h"
#include "SparseMatrix.h"


/* Dot product between two vectors */
double dot_product(double* vector1, double* vector2, int vector_size){
	double sum = 0;
	double product;
	int i;
	for (i = 0; i < vector_size; i++){
		product = vector1[i] * vector2[i];
		sum = sum + product;
	}

	return sum;
}

void print_vector(double* vector, int vector_size){
	int i;

	for (i = 0; i < vector_size; i++){
		printf("%f + , ", vector[i]);
	}

}
