/*
 * main.c
 *
 *  Created on: Aug 8, 2020
 *      Author: user
 */

#include <stdlib.h>
#include <stdio.h>
#include "NetworkDivision.h"
#include "LibFuncsHandler.h"

int main(int argc, char* argv[]){
	FILE* input = NULL;
	FILE* output = NULL;

	/* Arguments should include only an input filename and an output filename */
	if (argc != 3) {
		printf("Num of args should be 3\n");
		exit(EXIT_FAILURE);
	}
	input = open_file(argv[1], "rb");
	assert_file_not_empty(input);
	close_file(input);

	input = open_file(argv[1], "rb");
	output = open_file(argv[2], "wb");
	divide_net_to_clusters(input, output);
	close_file(input);
	close_file(output);

	/* TODO: delete these lines */
	output = open_file(argv[2], "rb");
	print_output_file(output);
	close_file(output);

	exit(EXIT_SUCCESS);
}
