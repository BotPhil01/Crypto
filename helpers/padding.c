#include"padding.h"


/*
 * padds an array of chars to a set size
 * @param pointer to array of chars
 * @return 0 if otherwise > 1
 * 
 */
// #define PADDING_PKCS5 8
// #define PADDING_PKCS7 16
int pad_bytes(uint8_t *bytes, size_t length, int mode) {
    int block_size = 1;
    switch(mode) {
        case PADDING_PKCS5:
            block_size = mode;
            break;
        case PADDING_PKCS7:
            block_size = mode;
            break;
        default:
            return block_size;
    }
    return 0;

}

