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

// void _mixCols(cipherInput *ciSrc) {
void _mixCols(block *bSrc) {
    block bTmp;
    bzero(bTmp.bytes, BLOCKSIZE);

    // isolate each col
    for (u32 i = 0; i < BLOCKDIMENSION; i++) {
        u8 uCol[BLOCKDIMENSION];
        bzero(uCol, BLOCKDIMENSION);
        for (u32 j = 0; j < BLOCKDIMENSION; j++) {
            uCol[j] = bSrc->bytes[j * BLOCKDIMENSION + i];
        }
        u8 uMCol[BLOCKDIMENSION];
        bzero(uMCol, BLOCKDIMENSION);
        
        for (u32 j = 0; j < BLOCKDIMENSION; j++) {
            const u8 u2Coefficient = uCol[j];
            const u8 u3Coefficient = uCol[(j + 1) % BLOCKDIMENSION];
            uMCol[j] = uCol[(j + 2) % BLOCKDIMENSION] ^ uCol[(j + 3) % BLOCKDIMENSION];
            uMCol[j] ^= uGaloisMult2[u2Coefficient] ^ uGaloisMult3[u3Coefficient];
        }
        
        for (u32 j = 0; j < BLOCKDIMENSION; j++) {
            bTmp.bytes[j * BLOCKDIMENSION + i] = uMCol[j];
        }
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

void _subBytes(block *bSrc) {
    for (u32 i = 0; i < BLOCKSIZE; i++) {
        bSrc->bytes[i] = subFwd(bSrc->bytes[i]);
    }
}

// seg faults in here
void _shiftRows(block *bSrc) {
    block bTmp;
    bzero(bTmp.bytes, BLOCKSIZE);

    // wtf is this
    
    for (u32 i = 0; i < BLOCKDIMENSION; i++) {
        const u32 uBlockOffset = i; // segfault here
        for (u32 j = 0; j < BLOCKDIMENSION; j++) {
            const u32 uDst = BLOCKDIMENSION * i + j;
            const u32 uSrc = BLOCKDIMENSION * i + ((i + j) % BLOCKDIMENSION);
            bTmp.bytes[uDst] = bSrc->bytes[uSrc];
        }
    }
    // printArr(bTmp.bytes, BLOCKSIZE);
    memcpy(bSrc->bytes, bTmp.bytes, BLOCKSIZE);
}

void _addRKey(block *bSrc, const rKey *rKey) {
    _bXor(bSrc, rKey);
}

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
