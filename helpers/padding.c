#include"padding.h"
#include<string.h>
#include<assert.h>

/*
 * @param bytes_length length of the array to be padded
 * @return size of buffer to be allocated for the padded array or -1 if arguments are invalid
 */
size_t pad_size_pkcs5(const size_t bytes_length) {
    return pad_size_pkcs7(bytes_length, 8);
}

size_t pad_size_pkcs7(const size_t bytes_length, const int block_size) {
    if (block_size < 1 || block_size > 255) {
        return -1;
    }
    return bytes_length + (block_size - bytes_length % block_size);
}

int pad_pkcs5(uint8_t *out, const uint8_t *bytes, const size_t bytes_length) {
    const int block_size = 8;
    return pad_pkcs7(out, bytes, bytes_length, block_size);
}

// correct
int pad_pkcs7(uint8_t *out, const uint8_t *bytes, const size_t bytes_length, const int block_size) {
    if (block_size < 1 || block_size > 255) {
        return -1;
    }
    const int diff = block_size * sizeof(uint8_t) - (bytes_length % block_size);
    memset(out, diff, bytes_length + diff);
    memcpy(out, bytes, bytes_length);
    return 0;
}
