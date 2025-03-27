#include <stddef.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define length(elem_s, arr_s) (arr_s / elem_s);

void char_to_hex(char *out, const uint8_t byte) {
    char lookup[] = "0123456789ABCDEF";
    uint8_t first = (byte & 0xF0) >> 4;
    uint8_t second = byte & 0x0F;
    sprintf(out, "0x%c%c", lookup[first], lookup[second]);
}

void bytes_to_hex_str(char *out, const uint8_t * const bytes, const size_t size) {
    int len = length(sizeof(bytes[0]), size);

    if (len == 0) {
        return;
    }

    char tmp[6];
    const size_t TMP_S = sizeof(tmp);
    const char DELIM[2] = ", ";
    const size_t DELIM_S = sizeof(DELIM);

    for (int i = 0; i < len; i++) {
        // create the char
        char_to_hex(tmp, bytes[i]);
        memcpy(tmp+4, DELIM, DELIM_S);
        if (i == len-1) {
            char final[5];
            bzero(final, 5);
            memcpy(final, tmp, 4);
            memcpy(out + (i*6), final, sizeof(final));
        } else {
            memcpy(out + (i*6), tmp, TMP_S);
        }
    }
}

int hex_str_len(const size_t bytes_size) {
    return ((bytes_size / sizeof(uint8_t)) - 1) * 6 + 4;
}

void word32_to_arr8(uint8_t *dest, const uint32_t word) {
    if (word == 0) {
        return;
    }
    uint32_t comparator = 0xFF000000;
    for (int i = 0; i < 4; i++) {
        uint32_t byte = comparator & word;
        uint8_t shifted = byte >> (8 * (3-i));
        dest[i] = shifted;
        // setup for next
        comparator = comparator >> 8;
    }
}

uint32_t arr8_to_word32(const uint8_t * const arr) {
    uint32_t ret = 0;
    for (int i = 0; i < 4; i++) {
        ret = ret | ((uint32_t) arr[i]) << (8 * (3 - i));
    }
    return ret;
}
