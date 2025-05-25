#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../helpers/headers/types.h"
#include "../helpers/headers/blocks.h"
#include "headers/ecb.h"
#include "headers/cbc.h"
#include "headers/ctr.h"

// ALL REQUIRE A KEY
// CBC, CTR REQUIRE AN IV

mode strToMode(const char *pInput, const size_t sInput) {
    assert(sInput > 2);
    if (pInput[0] == 'E') {
        return ECB;
    }
    if (pInput[1] == 'B') {
        return CBC;
    }
    return CTR;
}

void selectEncCipher(cipherPtr *pCipher, mode m, bool bEnc) {
    *pCipher = NULL;
    if (bEnc) {
        switch (m) {
            case ECB:
                *pCipher = &ecbEncrypt;
                break;
            case CBC:
                *pCipher = &cbcEncrypt;
                break;
            case CTR:
                *pCipher = &ctrEncrypt;
                break;
        }
    } else {
        switch (m) {
            case ECB:
                *pCipher = &ecbDecrypt;
                break;
            case CBC:
                *pCipher = &cbcDecrypt;
                break;
            case CTR:
                *pCipher = &ctrDecrypt;
                break;
        }
    }
}

void applyCipher(cipherSetup *cData) {
    assert(cData->cipher != NULL);
    // not valid for non ecb
    cipherInput cIn = {
        .keySchedule = &cData->keySchedule,
        .sBlocks = cData->sBlocks,
        .blocks = cData->blocks
    };
    cData->cipher(&cIn);
    bsTranspose(cIn.blocks, cIn.sBlocks);
}
