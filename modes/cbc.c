#include "../helpers/headers/types.h"
#include "../helpers/headers/cipher.h"
#include "../helpers/headers/blocks.h"
#include "../helpers/headers/transposition.h"
#include <string.h>

#include <stdio.h>
void cbcEncrypt(cipherInput *pInput) {
    if (pInput->sBlocks == 0) {
        printf("No blocks to encrypt");
        return;
    }
    u8 uInit[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
    };
    transpose(uInit, BLOCKDIMENSION);
    memcpy(pInput->initVector.bytes, uInit, BLOCKSIZE);
    bXor(pInput->blocks, &pInput->initVector);
    aesEncrypt(pInput->blocks, pInput->keySchedule);
    for (u32 i = 1; i < pInput->sBlocks; i++) {
        bXor(pInput->blocks + i, pInput->blocks + (i - 1));
        aesEncrypt(pInput->blocks, pInput->keySchedule);
    }
}

void cbcDecrypt(cipherInput *pInput) {
    if (pInput->sBlocks == 0) {
        printf("No blocks to decrypt");
        return;
    }
    u8 uInit[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
    };
    transpose(uInit, BLOCKDIMENSION);
    memcpy(pInput->initVector.bytes, uInit, BLOCKSIZE);
    for (u32 i = pInput->sBlocks - 1; i > 0; i--) {
        aesDecrypt(pInput->blocks + i, pInput->keySchedule);
        bXor(pInput->blocks + i, pInput->blocks + (i - 1));
    }
    aesDecrypt(pInput->blocks, pInput->keySchedule);
    bXor(pInput->blocks, &pInput->initVector);
}
