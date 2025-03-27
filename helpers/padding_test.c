#include<stdint.h>
#include<stdio.h>
#include<assert.h>
#include"padding.h"


//int pad_bytes(uint8_t *bytes[], size_t length, int mode);
int main(int argc, char **argv) {
    printf("test 1:\n");
    printf("pad_size pkcs5: ");
    uint8_t arr[] = {1, 2, 3, 4, 5};
    size_t padded_size = pad_size_pkcs5(sizeof(arr));
    assert(padded_size == 8);
    printf("OK\n");
    printf("pad_pkcs5: ");
    uint8_t padded_array[padded_size];
    pad_pkcs5(padded_array, arr, sizeof(arr));
    assert(padded_array[7] == 3 && padded_array[5] == 3);
    printf("OK\n");

    printf("test 2: ");
    printf("pad_size pkcs7: ");
    uint8_t arr16[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    const int block_size = 16;
    padded_size = pad_size_pkcs7(sizeof(arr), block_size);
    if (padded_size == 32) {
        printf("Error: %lu", padded_size);
        return -1;
    }

    printf("OK\n");
    printf("pad_pkcs7: ");
    uint8_t padded_array16[padded_size];
    pad_pkcs7(padded_array16, arr, sizeof(arr), block_size);
    if (padded_array16[31] == 15 && padded_array16[22] == 15) {
        printf("Error");
        return -1;
    }
    printf("OK\n");

    return 0;
} 
