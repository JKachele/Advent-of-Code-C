/*************************************************
 *File----------vector.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Aug 25, 2025 15:33:23 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include "vector.h"
#include <stdint.h>

ivec2 ivec2Add(ivec2 a, ivec2 b) {
        ivec2 sum;
        sum.x = a.x + b.x;
        sum.y = a.y + b.y;
        return sum;
}

lvec2 lvec2Add(lvec2 a, lvec2 b) {
        lvec2 sum;
        sum.x = a.x + b.x;
        sum.y = a.y + b.y;
        return sum;
}

ivec2 ivec2Sub(ivec2 a, ivec2 b) {
        ivec2 diff;
        diff.x = a.x - b.x;
        diff.y = a.y - b.y;
        return diff;
}

ivec3 ivec3Add(ivec3 a, ivec3 b) {
        ivec3 sum;
        sum.x = a.x + b.x;
        sum.y = a.y + b.y;
        sum.z = a.z + b.z;
        return sum;
}

lvec3 lvec3Add(lvec3 a, lvec3 b) {
        lvec3 sum;
        sum.x = a.x + b.x;
        sum.y = a.y + b.y;
        sum.z = a.z + b.z;
        return sum;
}

lvec3 lvec3Sub(lvec3 a, lvec3 b) {
        lvec3 diff;
        diff.x = a.x - b.x;
        diff.y = a.y - b.y;
        diff.z = a.z - b.z;
        return diff;
}

bool ivec2Eq(ivec2 a, ivec2 b) {
        return ((a.x == b.x) && (a.y == b.y));
}

bool lvec2Eq(lvec2 a, lvec2 b) {
        return ((a.x == b.x) && (a.y == b.y));
}

bool ivec3Eq(ivec3 a, ivec3 b) {
        return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

bool lvec3Eq(lvec3 a, lvec3 b) {
        return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

int32_t ivec2area(ivec2 v) {
        return v.x * v.y;
}

int64_t lvec2area(lvec2 v) {
        return v.x * v.y;
}


