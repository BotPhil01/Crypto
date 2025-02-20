#include<errno.h>
#include<stdint.h>
#ifdef BLOCK_CIPHER_H
#define BLOCK_CIPHER_H

/* Does block cipher encoding on the message
 * @params takes a message to encode and an out buffer to write to
 * @return void
 */

void block_cipher(uint8_t message[], uint8_t *out[]) {
    
}

/*
 * Does the s-box substitution of block cipher encoding
 * @params takes the padded message to encode and the out buffer to print it to
 * @returns void
 */
void sub_bytes(uint8_t message[], uint8_t *out[]) {}

/*
 * Does the shift rows part of block cipher
 * @params takes the padded message to encode and the out buffer to print it to
 * @returns void
 */
void shift_rows(uint8_t message[], uint8_t *out[]) {}

/*
 * Does the mix_columns of block_cipher encoding
 * @params takes the padded message to encode and the out buffer to print it to
 * @returns void
 */
void mix_columns(uint8_t message[], uint8_t *out[]) {}

/*
 * Does the add round key part of block cipher encoding 
 * @params takes the padded message to encode and the out buffer to print it to
 * @returns void
 */
void add_round_key(uint8_t message[], uint8_t *out[]) {}

#endif
