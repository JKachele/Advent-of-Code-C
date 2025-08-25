/*************************************************
 *File----------vector.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Aug 25, 2025 15:32:16 UTC
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

typedef struct {
        long x;
        long y;
} lvec2;

typedef struct {
        int x;
        int y;
} ivec2;

typedef struct {
        long x;
        long y;
        long z;
} lvec3;

typedef struct {
        int x;
        int y;
        int z;
} ivec3;

ivec2 addIVec2(ivec2 a, ivec2 b);
lvec2 addLVec2(lvec2 a, lvec2 b);
ivec3 addIVec3(ivec3 a, ivec3 b);
lvec3 addLVec3(lvec3 a, lvec3 b);
bool ivec2Eq(ivec2 a, ivec2 b);
bool lvec2Eq(lvec2 a, lvec2 b);
int ivec2area(ivec2 v);
long lvec2area(lvec2 v);

#endif

