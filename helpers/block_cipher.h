#include<errno.h>
#include<stdint.h>
#include<stddef.h>
#include<string.h>
#ifndef BLOCK_CIPHER_H
#define BLOCK_CIPHER_H

#define MAX_KEY_SIZE  8 * 4

typedef struct round_key_t {
    uint8_t key[MAX_KEY_SIZE];
    size_t key_size;
} round_key_t;

int get_key(uint8_t *target, round_key_t round_key);
/*
 * Struct for holding block cipher data
 * @params block this is the 16 byte block holding the data to be encrypted
 * @params key this is the key data in bytes
 * @params key_size this is the size of the key in bytes
 */
typedef struct block_cipher_data_t {
    uint8_t block[16];
    round_key_t round_key;
} block_cipher_data_t;


/* Does aes block cipher encoding on the message block
 * @params block cipher data
 * @return void
 */
extern int block_cipher(block_cipher_data_t *data);

/*
 * Does the s-box substitution of block cipher encoding
 * @params block cipher data
 * @returns void
 */
extern void sub_bytes(block_cipher_data_t *data);

/*
 * Does the inverse s-box substitution of block cipher encoding
 * @params block cipher data
 * @returns void
 */
extern void inv_sub_bytes(block_cipher_data_t *data);
/*
 * Does a circular left shift on the word for each byte
 * @params word to be shifted
 * @returns shifted word
 */
extern uint32_t rotate_word(uint32_t word);
/*
 * For each byte in the word it subs the byte for an sbox byte
 * @params word  to be substituted
 * @returns the substitued word
 */
extern uint32_t sub_word(uint32_t word);

/*
 * Does the shift rows part of block cipher
 * @params block cipher data
 * @returns void
 */
extern void shift_rows(block_cipher_data_t *data);

/*
 * Does the shift rows part of block cipher
 * @params block cipher data
 * @returns void
 */
extern void inv_shift_rows(block_cipher_data_t *data);

/*
 * Does the mix_columns of block_cipher encoding
 * @params block cipher data
 * @returns void
 */
extern void mix_cols(block_cipher_data_t *data);

/*
 * Does the inverse of mix_columns of block_cipher encoding
 * @params block cipher data
 * @returns void
 */
extern void inv_mix_cols(block_cipher_data_t *data);


/*
 * @params block cipher data
 * @returns the number of round keys to be generated
 */
extern int rkeys_len(const block_cipher_data_t *data);

/*
 * Generates the round keys from the main key given to the block cipher
 * @params block cipher data
 * @returns the number of round keys in the buffer
 */ 
extern void gen_rkeys(const block_cipher_data_t *data, uint32_t *rkeys_buffer);

/*
 * Does the add round key part of block cipher encoding in place
 * @params block cipher data
 * @returns void
 */
extern void add_rkey(block_cipher_data_t *data, const uint32_t rkey[]);

void mix_col(uint8_t *dest, const uint8_t * const src);
#endif
