// types for global usage across project
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "consts.h"


#ifndef TYPES_c
#define TYPES_c
// integers
typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;
typedef int8_t i8;

typedef char *str;

typedef struct block {
    u8 bytes[BLOCKSIZE];
} block;

typedef enum eUsage {
    UGENERAL,
    UKEYSIZE,
    UFILENOTFOUND,
    UKEY,
    UINPUTSIZE,
} eUsage;

typedef struct key {
    u8 uKey[32];
    size_t sKeySize;
} key;

typedef struct strInput {
    char cVal[SMAXSTRINPUT];
} strInput;

typedef struct InputData {
    strInput inPlaintext;
    strInput inCiphertext;
    strInput inKey;
    size_t sKeySize;
    strInput inMode;
    bool bHex;
} InputData;

typedef enum mode {
    CBC,
    ECB,
    CTR,
} mode;

typedef block rKey;
typedef struct keySchedule {
    key kInit;
    rKey rpRKeys[MAXRKEYS];
    size_t sRKeys;
} keySchedule;

typedef struct cipherInput {
    block *blocks;
    size_t sBlocks;
    const keySchedule *keySchedule;
    struct block initVector;
} cipherInput;

typedef void (*cipherPtr) (cipherInput *);

typedef struct cipherSetup {
    cipherPtr cipher;
    block *blocks;
    size_t sBlocks;
    keySchedule keySchedule;
    struct block initVector;
} cipherSetup;

#endif

