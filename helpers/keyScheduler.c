#include "headers/types.h"
#include "headers/consts.h"
#include "headers/substitution.h"
#include "headers/printer.h"
#include "headers/hex.h"
#include "headers/transposition.h"
#include "headers/blocks.h"
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>

void _rotate(u8 *uDst, const u8 *uSrc) {
    uDst[0] = uSrc[1];
    uDst[1] = uSrc[2];
    uDst[2] = uSrc[3];
    uDst[3] = uSrc[0];
}

void _transpose(keySchedule *ksDst) {
    for (u32 i = 0; i < ksDst->sRKeys; i++) {
        rKey *rkpCurr = ksDst->rpRKeys + i;
        transpose(rkpCurr->bytes, RKEYDIMENSION);
    }
}

void _xor(u8 *uDst, const u8 *uSrc1, const u8 *uSrc2) {
    for (u32 i = 0; i < 4; i++) {
        uDst[i] = uSrc1[i] ^ uSrc2[i];
    }
}

void _wSubFwd(u8 *uDst, const u8 *uSrc) {
    for (u32 i = 0; i < 4; i++) {
        uDst[i] = subFwd(uSrc[i]);
    }
}
// convert a key string to an array
void katob(key *kDst, const InputData *idSrc) {
    bzero(kDst->uKey, MAXKEYSIZE); 
    kDst->sKeySize = idSrc->sKeySize;
    if (idSrc->bHex) {
        u8 uTmp[MAXKEYSIZE * 2];
        memcpy(uTmp, idSrc->inKey.cVal, idSrc->sKeySize * 2);
        ascToHex(uTmp, idSrc->sKeySize * 2);
        memcpy(kDst->uKey, uTmp, idSrc->sKeySize);
    } else {
        memcpy(kDst->uKey, idSrc->inKey.cVal, idSrc->sKeySize);
    }
}

void createRKeys(keySchedule *ksSchedule) {
    for (u32 i = 0; i < MAXRKEYS; i++) {
        explicit_bzero(&ksSchedule->rpRKeys[i], RKEYSIZE); 
    }
    // work in layers
    // size of initial key in 32 bit words
    size_t sInitKey32 = ksSchedule->kInit.sKeySize / BLOCKDIMENSION;
    // calculate how many rkeys required
    size_t sRKeys = 0;
    switch (ksSchedule->kInit.sKeySize) {
        case 16:
            sRKeys = 11;
            break;
        case 24:
            sRKeys = 13;
            break;
        case 32:
            sRKeys = 15;
            break;
    }
    assert(sRKeys != 0);

    ksSchedule->sRKeys = sRKeys;

    const size_t sExpandedKey = BLOCKDIMENSION * BLOCKDIMENSION * sRKeys;
    u8 uExpandedKey[sExpandedKey];
    bzero(uExpandedKey, sExpandedKey);

    const size_t sWord32 = 4;

    struct word32 {
        u8 bytes[sWord32];
    };

    struct expandedKey {
        struct word32 words[MAXRKEYS * 4];
    };

    struct expandedKey expKey;
    const size_t sWords = sRKeys * 4;

    for (u32 i = 0; i < sWords; i++) {
        const u32 uRem = i % sInitKey32;
        struct word32 wWorking;
        if (i < sInitKey32) {
            memcpy(wWorking.bytes, ksSchedule->kInit.uKey + i * sWord32, sWord32);
        } else if (uRem == 0) {
            struct word32 wRCon;
            memcpy(wRCon.bytes, uRConsts[(i / sInitKey32) - 1], sWord32);

            struct word32 wRot;
            _rotate(wRot.bytes, expKey.words[i - 1].bytes);

            struct word32 wSub; 
            _wSubFwd(wSub.bytes, wRot.bytes);

            struct word32 wXord;
            _xor(wXord.bytes, wRCon.bytes, wSub.bytes);
            memcpy(wWorking.bytes, expKey.words[i - sInitKey32].bytes, sWord32);
            _xor(wWorking.bytes, wXord.bytes, wWorking.bytes);

        } else if (sInitKey32 > 6 && uRem == 4) {
            memcpy(wWorking.bytes, expKey.words[i - 1].bytes, sWord32);

            struct word32 wSub;
            _wSubFwd(wSub.bytes, wWorking.bytes);

            memcpy(wWorking.bytes, expKey.words[i - sInitKey32].bytes, sWord32);
            struct word32 wXord;
            _xor(wXord.bytes, wSub.bytes, wWorking.bytes);

            memcpy(wWorking.bytes, wXord.bytes, sWord32);
        } else {
            memcpy(wWorking.bytes, expKey.words[i - sInitKey32].bytes, sWord32);
            struct word32 wXord;
            _xor(wXord.bytes, wWorking.bytes, expKey.words[i - 1].bytes);

            memcpy(wWorking.bytes, wXord.bytes, sWord32);
        }
        memcpy(expKey.words[i].bytes, wWorking.bytes, sWord32);
    }

    // compress into round Keys
    for (u32 i = 0; i < sRKeys; i++) {
        block bTmp;
        bzero(bTmp.bytes, BLOCKSIZE);
        for (u32 j = 0; j < BLOCKDIMENSION; j++) {
            memcpy(bTmp.bytes + j * 4, expKey.words + 4 * i + j, 4);
        }
        bzero(ksSchedule->rpRKeys[i].bytes, 16);
        memcpy(ksSchedule->rpRKeys[i].bytes, bTmp.bytes, 16);

        // transpose
        // bsTranspose(ksSchedule->rpRKeys + i, BLOCKDIMENSION);
    }
}
