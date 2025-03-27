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
 */
#define PADDING_PKCS5 8
#define PADDING_PKCS7 16

/*
 * returns the size of the padded msg in bytes
 * @params size of plaintext in bytes
 * @returns size of padded plaintext in bytes
 */
size_t pad_size_pkcs5(const size_t bytes_length);

/*
 * returns the size of the padded msg in bytes
 * @params size of plaintext in bytes
 * @params size of a block in bytes
 * @returns size of padded plaintext in bytes
 */
size_t pad_size_pkcs7(const size_t bytes_length, const int block_size);

/*
 * padds an array of chars to a set size
 * @param out buffer to write the padded bytes to 
 * @param bytes buffer to read the bytes from
 * @param bytes_length amount of bytes to read from bytes
 * @return 0 if successfuly else !0 
 */
int pad_pkcs5(uint8_t *out, const uint8_t *bytes, const size_t bytes_length);

/*
 * padds an array of chars to a set size
 * @param out -> pointer to a preallocated array for storing the result
 * @param bytes -> pointer to array of chars to pad
 * @param bytes_length length of the array
 * @param int block_size
 * @return 0 if successfuly else !0 
 */
int pad_pkcs7(uint8_t *out, const uint8_t *bytes, const size_t bytes_length, const int block_size);
#endif

