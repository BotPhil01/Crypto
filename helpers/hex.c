#include "headers/types.h"
#include <assert.h>
#include <string.h>

u8 _cAscToHex(u8 c) {
    if (c > 0x60 && c < 0x7b) {
        // letters
        return c - 0x57;
    } else if (c > 0x40 && c < 0x5b) {
        return c - 55;
    } else if (c > 0x2f && c < 0x3a) {
        // numbers
        return c - 0x30;
    }
    return 0;
}

void ascToHex(u8 *pBytes, const size_t sBytes) {

    assert(sBytes % 2 == 0);
    for (u32 i = 0; i < sBytes / 2; i++) {
        const u32 index = i * 2;
        u8 uByte = _cAscToHex(pBytes[index]);
        uByte = uByte << 4;
        uByte = uByte | _cAscToHex(pBytes[index + 1]);
        pBytes[i] = uByte;
    }
    bzero(pBytes + sBytes / 2, sBytes / 2);
}
