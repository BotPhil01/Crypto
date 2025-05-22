#include <stdint.h>

#ifndef CONSTS_H
#define CONSTS_H

#define BLOCKSIZE 16
#define RKEYSIZE BLOCKSIZE
#define BLOCKDIMENSION 4
#define RKEYDIMENSION BLOCKDIMENSION
#define MAXKEYSIZE 32
#define MAXRKEYS 15
#define MAXFILENAME 255
#define SMAXSTRINPUT UINT8_MAX


extern const uint8_t uRConsts[10][4];

extern const uint8_t uFwdSBox[256];

extern const uint8_t uBwdSBox[256];

extern const uint8_t uGaloisMult2[256];

extern const uint8_t uGaloisMult3[256];

extern const uint8_t uGaloisMult9[256];

extern const uint8_t uGaloisMult11[256];

extern const uint8_t uGaloisMult13[256];

extern const uint8_t uGaloisMult14[256];
#endif
