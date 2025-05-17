#include "types.h"

#ifndef PADDING_C
#define PADDING_C 
/*
 *@Param bpBlock pointer to block to pad
 *@Param iPadCount number of bytes to pad
 */
extern void padPkcs7(block *bpBlock, const i8 iPadCount);
#endif
