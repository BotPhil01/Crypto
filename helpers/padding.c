#include "headers/types.h"
#include "headers/printer.h"
#include <stdio.h>
#include <string.h>

#ifndef PADDING_C
#define PADDING_C 
/*
 *@Param bpBlock pointer to block to pad
 *@Param iPadCount number of bytes to pad
 */
void padPkcs7(block *bpBlock, const i8 iPadCount) {
    memset((*bpBlock) + BLOCKSIZE - iPadCount, iPadCount, iPadCount);
}
#endif
