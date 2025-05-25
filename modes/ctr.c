#include "../helpers/headers/types.h"
#include "../helpers/headers/blocks.h"
#include "../helpers/headers/cipher.h"
#include "../helpers/headers/transposition.h"
#include <strings.h>
#include <string.h>
#include <stdio.h>

void ctrEncrypt(cipherInput *pInput) {
    const block bVals = {
        .bytes = {
            0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
            0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
        }
    };

    // set init vector
    memcpy(&pInput->initVector.bytes, &bVals.bytes, BLOCKSIZE);

    // ctr encrypts 
    // b ^ enc(ctr + iv)
    // block is 8 * 16
    // check if nonce is 0

    // if (bZeroCheck(&pInput->initVector)) {
    //     printf("Bad nonce: don't use zeroed nonce");
    //     return;
    // }

    block bCtr;
    bzero(&bCtr.bytes, BLOCKSIZE);

    for (u32 i = 0; i < pInput->sBlocks; i++) {
        block bNonce;
        memcpy(&bNonce.bytes, &pInput->initVector, BLOCKSIZE);
        bXor(&bNonce, &bCtr);
        aesEncrypt(&bNonce, pInput->keySchedule);
        bXor(pInput->blocks + i, &bNonce);
        bIncrement(&bCtr);
    }
}

void ctrDecrypt(cipherInput *pInput) {
    ctrEncrypt(pInput);
}
