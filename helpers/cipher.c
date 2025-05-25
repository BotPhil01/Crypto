#include "../helpers/headers/types.h"
#include "../helpers/headers/consts.h"
#include "../helpers/headers/substitution.h"
#include "../helpers/headers/printer.h"
#include <strings.h>
#include <string.h>

// xors in place
void _bXor(block *bDst, const block *bSrc) {
    for (u32 i = 0; i < BLOCKSIZE; i++) {
        bDst->bytes[i] = bDst->bytes[i] ^ bSrc->bytes[i];
    }
}

// cols -> rows
// rows -> cols
void _mixCols(block *bSrc) {
    block bTmp;
    bzero(bTmp.bytes, BLOCKSIZE);

    // isolate each col
    for (u32 i = 0; i < BLOCKDIMENSION; i++) {
        const u8 uColStartIndex = BLOCKDIMENSION * i;
        u8 uCol[BLOCKDIMENSION];
        bzero(uCol, BLOCKDIMENSION);
        memcpy(uCol, bSrc->bytes + uColStartIndex, BLOCKDIMENSION);

        u8 uMCol[BLOCKDIMENSION];
        bzero(uMCol, BLOCKDIMENSION);
        
        for (u32 j = 0; j < BLOCKDIMENSION; j++) {
            const u8 u2Coefficient = uCol[j];
            const u8 u3Coefficient = uCol[(j + 1) % BLOCKDIMENSION];
            uMCol[j] = uCol[(j + 2) % BLOCKDIMENSION] ^ uCol[(j + 3) % BLOCKDIMENSION];
            uMCol[j] ^= uGaloisMult2[u2Coefficient] ^ uGaloisMult3[u3Coefficient];
        }
        
        memcpy(bTmp.bytes + uColStartIndex, uMCol, BLOCKDIMENSION);
    }
    // for each row
    // apply the matrix
    // d = 2 3 1 1
    // d = 1 2 3 1
    // d = 1 1 2 3
    // d = 3 1 1 2
    // printArr(bTmp.bytes, BLOCKDIMENSION);
    memcpy(bSrc->bytes, bTmp.bytes, BLOCKSIZE);
}

void _iMixCols(block *bSrc) {
    block bTmp;
    bzero(bTmp.bytes, BLOCKSIZE);

    // isolate each col
    for (u32 i = 0; i < BLOCKDIMENSION; i++) {
        const u8 uColStartIndex = BLOCKDIMENSION * i;
        u8 uCol[BLOCKDIMENSION];
        bzero(uCol, BLOCKDIMENSION);
        memcpy(uCol, bSrc->bytes + uColStartIndex, BLOCKDIMENSION);

        u8 uMCol[BLOCKDIMENSION];
        bzero(uMCol, BLOCKDIMENSION);
        
        for (u32 j = 0; j < BLOCKDIMENSION; j++) {
            const u8 u14Coefficient = uCol[j];
            const u8 u11Coefficient = uCol[(j + 1) % BLOCKDIMENSION];
            const u8 u13Coefficient = uCol[(j + 2) % BLOCKDIMENSION];
            const u8 u9Coefficient = uCol[(j + 3) % BLOCKDIMENSION];
            uMCol[j] = uGaloisMult14[u14Coefficient];
            uMCol[j] ^= uGaloisMult11[u11Coefficient];
            uMCol[j] ^= uGaloisMult13[u13Coefficient];
            uMCol[j] ^= uGaloisMult9[u9Coefficient];
        }
        
        memcpy(bTmp.bytes + uColStartIndex, uMCol, BLOCKDIMENSION);
    }
    memcpy(bSrc->bytes, bTmp.bytes, BLOCKSIZE);
}

void _subBytes(block *bSrc) {
    for (u32 i = 0; i < BLOCKSIZE; i++) {
        bSrc->bytes[i] = subFwd(bSrc->bytes[i]);
    }
}

void _iSubBytes(block *bSrc) {
    for (u32 i = 0; i < BLOCKSIZE; i++) {
        bSrc->bytes[i] = subBwd(bSrc->bytes[i]);
    }
}

void _shiftRows(block *bSrc) {
    block bTmp;
    bzero(bTmp.bytes, BLOCKSIZE);
    memcpy(bTmp.bytes, bSrc->bytes, BLOCKSIZE);

    for (u32 i = 0; i < BLOCKSIZE; i++) {
        const u32 uSrcIndex = uFwdSR[i];
        bSrc->bytes[i] = bTmp.bytes[uSrcIndex];
    }
}

void _iShiftRows(block *bSrc) {
    block bTmp;
    bzero(bTmp.bytes, BLOCKSIZE);
    memcpy(bTmp.bytes, bSrc->bytes, BLOCKSIZE);

    for (u32 i = 0; i < BLOCKSIZE; i++) {
        const u32 uSrcIndex = uBwdSR[i];
        bSrc->bytes[i] = bTmp.bytes[uSrcIndex];
    }
}

void _addRKey(block *bSrc, const rKey *rKey) {
    _bXor(bSrc, rKey);
}

#define _iAddRKey(bSrc, rKey) _addRKey(bSrc, rKey)

void aesEncrypt(block *ciSrc, const keySchedule *ksSchedule) {
    _addRKey(ciSrc, &ksSchedule->rpRKeys[0]);
    for (u32 i = 1; i < ksSchedule->sRKeys - 1; i++) {
        _subBytes(ciSrc);
        _shiftRows(ciSrc);
        _mixCols(ciSrc);
        _addRKey(ciSrc, &ksSchedule->rpRKeys[i]);
    }
    _subBytes(ciSrc);
    _shiftRows(ciSrc);
    _addRKey(ciSrc, &ksSchedule->rpRKeys[ksSchedule->sRKeys - 1]);
}

void aesDecrypt(block *ciSrc, const keySchedule *ksSchedule) {
    _iAddRKey(ciSrc, &ksSchedule->rpRKeys[ksSchedule->sRKeys - 1]);
    _iShiftRows(ciSrc);
    _iSubBytes(ciSrc);
    for (u32 i = ksSchedule->sRKeys - 2; i > 0; i--) {
        _iAddRKey(ciSrc, &ksSchedule->rpRKeys[i]);
        _iMixCols(ciSrc);
        _iShiftRows(ciSrc);
        _iSubBytes(ciSrc);
    }
    _iAddRKey(ciSrc, &ksSchedule->rpRKeys[0]);
}
