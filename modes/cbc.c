#include "../helpers/headers/types.h"
#include "../helpers/headers/cipher.h"
#include "../helpers/headers/blocks.h"

#include <stdio.h>
void cbcEncrypt(cipherInput *pInput) {
    if (pInput->sBlocks == 0) {
        printf("No blocks to encrypt");
        return;
    }
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
    for (u32 i = pInput->sBlocks - 1; i > 0; i--) {
        aesDecrypt(pInput->blocks + i, pInput->keySchedule);
        bXor(pInput->blocks + i, pInput->blocks + (i - 1));
    }
    aesDecrypt(pInput->blocks, pInput->keySchedule);
    bXor(pInput->blocks, &pInput->initVector);
}
