#include "headers/types.h"
#include "headers/padding.h"
#include "headers/printer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef BLOCKS_C
#define BLOCKS_C
// splits data into blocks
i32 blockCount(const size_t sFile) {
    return (sFile / BLOCKSIZE) + 1;
}

void fillBlocks(block *bppBlocks, i8 *ippBytes, const size_t sBytes) {
    const i32 iBlockCount = blockCount(sBytes);
    i32 iByteOffset = 0;
    i32 iBlockCounter = 0;

    // iterate through bytes
    // read each byte and place in associated block

    while (iByteOffset < sBytes) {
        bzero(bppBlocks[iBlockCounter], BLOCKSIZE);
        // if block is complete copy whole block
        u32 uCopyCount = BLOCKSIZE;
        if (sBytes - iByteOffset < 16) {
            uCopyCount = sBytes - iByteOffset;
        }
        memcpy(bppBlocks[iBlockCounter], ippBytes + iByteOffset, uCopyCount);
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
        memcpy(iCBytes + iBytesOffset, pBlocks[iBlocksCounter], BLOCKSIZE);
        iBlocksCounter++;
        iBytesOffset = iBlocksCounter * BLOCKSIZE;
    }
}
#endif
