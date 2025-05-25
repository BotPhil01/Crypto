#include "types.h"
#include "padding.h"
#include <stdlib.h>

#ifndef BLOCKS_C
#define BLOCKS_C

extern i32 blockCount(const size_t sFile);

/*
 *@Param ippBlcoks pointer to list of blocks
 *@Param ipBytes pointer to list of bytes
 */
extern void strToBlocks(block *bppBlocks, u8 *ippBytes, const size_t sBytes);

/*
 *@Param ippBlocks pointer to block
 *@Param ipBytes pointer to list of bytes
 */
extern void strToBlock(block *bppBlocks, u8 *ippBytes, const size_t sBytes);

/*
 *@Param pBytes list of bytes
 *@Param pBlocks blocks list
 *@Param sBlocks blocks size
 */
extern void blocks2Bytes(u8 *iCBytes, block *pBlocks, const size_t sBlocks);
extern void bsTranspose(block *bppDst, const size_t sBlocks);
extern int bIncrement(block *bDst);
extern void bXor(block *bDst, const block *bSrc);
extern int bZeroCheck(const block *bDst);
#endif
