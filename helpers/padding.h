// implements pkcs/5 and pkcs/7
#include<stdint.h>
#include<stdio.h>
#include<errno.h>


#ifndef PADDING_H
#define PADDING_H

/*
 * padds an array of chars to a set size
 * @param pointer to array of chars
 * @
 *
 */
#define PADDING_PKCS5 1
#define PADDING_PKCS7 2
int pad_bytes(uint8_t *bytes, size_t length, int mode);



#endif

