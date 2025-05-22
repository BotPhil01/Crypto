#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../helpers/headers/types.h"
#include "../helpers/headers/blocks.h"
#include "headers/ecb.h"

// ALL REQUIRE A KEY
// CBC, CTR REQUIRE AN IV

mode strToMode(const char *pInput, const size_t sInput) {
    return ECB;
    assert(sInput > 3);
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
                break;
            case CTR:
                break;
        }
    } else {
        switch (m) {
            case ECB:
                *pCipher = &ecbDecrypt;
                break;
            case CBC:
                break;
            case CTR:
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
