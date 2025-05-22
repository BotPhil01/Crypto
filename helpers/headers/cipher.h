#include "types.h"
#ifndef CIPHER_H
#define CIPHER_H

extern void aesEncrypt(block *ciSrc, const keySchedule *ksSchedule);
extern void _addRKey(block *bSrc, const rKey *rKey);
extern void _shiftRows(block *bSrc);
extern void _subBytes(block *bSrc);
extern void _mixCols(block *bSrc);
#endif
