/*************************************************
 *File----------util.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 17, 2024 08:13:18 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* #define max(a,b) \
    ({ __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; })
*/

// Nested For Loops
#define MAKE_LOOP_1(i, mi)  for(int i = 0; i < mi; i++)
#define MAKE_LOOP_2(i, mi, ...) MAKE_LOOP_1(i, mi) MAKE_LOOP_1(__VA_ARGS__)
#define MAKE_LOOP_3(i, mi, ...) MAKE_LOOP_1(i, mi) MAKE_LOOP_2(__VA_ARGS__)
#define MAKE_LOOP_4(i, mi, ...) MAKE_LOOP_1(i, mi) MAKE_LOOP_3(__VA_ARGS__)
// etc.
#define MAKE_LOOP_N(_8, _7, _6, _5, _4, _3, _2, _1, N, ...) MAKE_LOOP_##N
#define MAKE_LOOP(...) MAKE_LOOP_N(__VA_ARGS__, 4,4,3,3,2,2,1,1)(__VA_ARGS__)

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef struct {
        int64 x;
        int64 y;
} lvec2;

typedef struct {
        int32 x;
        int32 y;
} ivec2;

typedef struct {
        int64 x;
        int64 y;
        int64 z;
} lvec3;

typedef struct {
        int32 x;
        int32 y;
        int32 z;
} ivec3;

typedef enum {
        NORTH,
        EAST,
        SOUTH,
        WEST
} direction;

bool isDigit(char c);
void printInt(void *data);
void printLong(void *data);
int64_t ipow(int64_t base, uint8_t exp);
ivec2 addIVec2(ivec2 a, ivec2 b);
lvec2 addLVec2(lvec2 a, lvec2 b);
ivec3 addIVec3(ivec3 a, ivec3 b);
lvec3 addLVec3(lvec3 a, lvec3 b);
bool ivec2Eq(ivec2 a, ivec2 b);
bool lvec2Eq(lvec2 a, lvec2 b);
uint64 computeHash(char const *input);


#endif

