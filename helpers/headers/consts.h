#include <stdint.h>

#ifndef CONSTS_H
#define CONSTS_H

#define BLOCKSIZE 16
#define BLOCKDIMENSION 4
#define MAXKEYSIZE 32
#define MAXRKEYS 15
#define MAXFILENAME 255

extern const uint8_t uRConsts[10][4];

extern const uint8_t uFwdSBox[256];

extern const uint8_t uBwdSBox[256];
#endif
