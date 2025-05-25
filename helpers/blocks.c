#include "headers/types.h"
#include "headers/padding.h"
#include "headers/printer.h"
#include "headers/transposition.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#ifndef BLOCKS_C
#define BLOCKS_C
// splits data into blocks
u32 blockCount(const size_t sFile) {
    return (sFile / BLOCKSIZE) + 1;
}

void fillBlocks(block *bppBlocks, i8 *ippBytes, const size_t sBytes) {
    // FILLS BLOCKS INCORRECTLY
    // SHOULD FILL COLUMNS FIRST THEN ROWS
    const i32 iBlockCount = blockCount(sBytes);
    i32 iByteOffset = 0;
    i32 iBlockCounter = 0;

    // iterate through bytes
    // read each byte and place in associated block

    while (iByteOffset < sBytes) {
        bzero(bppBlocks[iBlockCounter].bytes, BLOCKSIZE);
        // if block is complete copy whole block
        u32 uCopyCount = BLOCKSIZE;
        if (sBytes - iByteOffset < 16) {
            uCopyCount = sBytes - iByteOffset;
        }
        // TODO change later to fill columns then rows
        for (u32 i = 0; i < BLOCKDIMENSION; i++) {
            for (u32 j = 0; j < BLOCKDIMENSION; j++) {
                const u8 byte = (ippBytes + iByteOffset)[BLOCKDIMENSION * i + j];
                bppBlocks[iBlockCounter].bytes[BLOCKDIMENSION * j + i] = byte;
            }
        }
        // memcpy(bppBlocks[iBlockCounter].bytes, ippBytes + iByteOffset, uCopyCount);
        iBlockCounter++;
        iByteOffset = iBlockCounter * BLOCKSIZE;
    }

    const i32 iPadCount = BLOCKSIZE - (sBytes % BLOCKSIZE);
    const bool bPaddingRequired = iPadCount != 16;
    if (bPaddingRequired) {
        padPkcs7(&(bppBlocks[iBlockCount-1]), iPadCount);
    }
}

void blocks2Bytes(i8 *iCBytes, block *pBlocks, const size_t sBlocks) {
    i32 iBytesOffset = 0;
    i32 iBlocksCounter = 0;
    while (iBlocksCounter != sBlocks) {
        // copy block into bytes
        memcpy(iCBytes + iBytesOffset, pBlocks[iBlocksCounter].bytes, BLOCKSIZE);
        iBlocksCounter++;
        iBytesOffset = iBlocksCounter * BLOCKSIZE;
    }
}

void bsTranspose(block *bppDst, const size_t sBlocks) {
    for (u32 i = 0; i < sBlocks; i++) {
        transpose(bppDst[i].bytes, BLOCKDIMENSION);
    }
}

int _incrementHelper(block *bDst, const u32 uIndex) {
    if (bDst->bytes[uIndex] == 0xff) {
        bDst->bytes[uIndex] = 0;
        // overflow error
        if (uIndex == 0) {
            return 1;
        }
        return _incrementHelper(bDst, uIndex-1);
    }
    bDst->bytes[uIndex]++;
    return 0;
}

int bIncrement(block *bDst) {
    return _incrementHelper(bDst, BLOCKSIZE - 1);
}

void bXor(block *bDst, const block *bSrc) {
    for (u32 i = 0; i < BLOCKSIZE; i++) {
        bDst->bytes[i] = bDst->bytes[i] ^ bSrc->bytes[i];
    }
}

int bZeroCheck(const block *bDst) {
    for (u32 i = 0; i < BLOCKSIZE; i++) {
        if (bDst->bytes[i] != 0) {
            return 1;
        }
    }
    return 0;
}
#endif
