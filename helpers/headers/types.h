// types for global usage across project
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
} eUsage;

typedef struct key {
    u8 uKey[32];
    size_t sKeySize;
} key;

typedef struct InputData {
    char *cpPlaintext;
    char *cpCiphertext;
    char *cpKey;
    size_t sKeySize;
} InputData;

typedef enum mode {
    CBC,
    ECB,
    CTR,
} mode;

typedef struct cipherInput {
    block block;
    key key;
    block initVector;
} cipherInput;

typedef block rKey;
typedef struct keySchedule {
    key kInit;
    rKey rpRKeys[15];
    size_t sRKeys;
} keySchedule;
#endif

