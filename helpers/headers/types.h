// types for global usage across project
#include <stdint.h>
#include <stdbool.h>

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

#define BLOCKSIZE 16
typedef i8 block[BLOCKSIZE];
#endif
