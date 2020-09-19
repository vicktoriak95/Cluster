/*
 * main.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "NetworkDivision.h"
#include "LibFuncsHandler.h"

#define DEBUG
#define PRINT_RESULT

#ifdef DEBUG
#include <time.h>
#endif

int old_main(int argc, char* argv[]){
	FILE* input = NULL;
	FILE* output = NULL;
	char* input_file_path = NULL;
	char* output_file_path = NULL;

	#ifdef DEBUG
	clock_t start, end;
	start = clock();
	#endif

	/* Arguments should include only an input filename and an output filename */
	if (argc != 3) {
		printf("Num of args should be 3\n");
		exit(EXIT_FAILURE);
	}
	input_file_path = argv[1];
	output_file_path = argv[2];

	/* Opening file to assert is not empty */
	input = open_file(input_file_path, "rb");
	assert_file_not_empty(input);
	close_file(input);

	/* Opening file to read network */
	input = open_file(input_file_path, "rb");
	output = open_file(output_file_path, "wb");
	divide_net_to_clusters(input, output);
	close_file(input);
	close_file(output);

	#ifdef DEBUG
	end = clock();
	printf("All execution took: %f seconds\n", ((double)(end-start) / CLOCKS_PER_SEC));

	#ifdef PRINT_RESULT
	output = open_file(output_file_path, "rb");
	print_output_file(output);
	close_file(output);
	#endif

	#endif

	exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
	FILE* input = NULL;
	char* input_file_path = NULL;
	Network* net;
	spmat* A1 = NULL;
	spmat* A2 = NULL;

	double s[10] = {1, -1, -1, 1, 1, 1, -1, 1, 1, -1};

	/* Arguments should include only an input filename and an output filename */
	if (argc != 3) {
		printf("Num of args should be 3\n");
		exit(EXIT_FAILURE);
	}
	input_file_path = argv[1];

	/* Opening file to assert is not empty */
	input = open_file(input_file_path, "rb");
	assert_file_not_empty(input);
	close_file(input);

	input = open_file(input_file_path, "rb");
	net = create_network(input);
	close_file(input);
	print_sparse_matrix(net->A);
	divide_spmat(net->A, s, &A1, &A2);
	print_sparse_matrix(A1);
	print_sparse_matrix(A2);

	exit(EXIT_SUCCESS);
}
