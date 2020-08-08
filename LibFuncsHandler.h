/*
 * LibFuncsHandler.h
 *
 *  Created on: 8 баев„ 2020
 *      Author: user
 */

#ifndef LIBFUNCSHANDLER_H_
#define LIBFUNCSHANDLER_H_

/* read an integer from an open file descriptor */
int int_fread(FILE* f);

/* allocate n bytes and return a pointer to the allocated space */
void* allocate(size_t n);

#endif /* LIBFUNCSHANDLER_H_ */
