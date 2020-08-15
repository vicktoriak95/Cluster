/*
 * LibFuncsHandler.h
 *
 *  Created on: 8 баев„ 2020
 *      Author: user
 */

#ifndef LIBFUNCSHANDLER_H_
#define LIBFUNCSHANDLER_H_

FILE* open_file(const char *pathname, const char *mode);

int close_file(FILE *stream);

/* Read an integer from an open file descriptor */
int int_fread(FILE* f);

/* Write an integer to an open file descriptor */
void int_fwrite(int n, FILE* f);

/* Allocate n bytes and return a pointer to the allocated space */
void* allocate(size_t n);

/* Detects infinite loops and exits */
void infinite_loop_detection(int counter, int max_counter);

#endif /* LIBFUNCSHANDLER_H_ */
