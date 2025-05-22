#include "headers/types.h"
#include <string.h>

void transpose(u8 *uBytes, const size_t sDimension) {
    const size_t sTotal = sDimension * sDimension;
    u8 uTmp[sTotal];
    memcpy(uTmp, uBytes, sTotal);
    for (u32 i = 0; i < sDimension; i++) {
        for (u32 j = 0; j < sDimension; j++) {
            const u8 byte = uTmp[sDimension * i + j];
            uBytes[sDimension * j + i] = byte;
        }
    }
}
