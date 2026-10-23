/*************************************************
 *File----------vector.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Aug 25, 2025 15:33:23 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include "vector.h"
#include <stdint.h>

/************************ 2D ************************/
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
lvec2 lvec2Sub(lvec2 a, lvec2 b) {
        lvec2 diff;
        diff.x = a.x - b.x;
        diff.y = a.y - b.y;
        return diff;
}
ivec2 ivec2Scale(ivec2 vec, int32_t scale) {
        ivec2 prod;
        prod.x = vec.x * scale;
        prod.y = vec.y * scale;
        return prod;
}
lvec2 lvec2Scale(lvec2 vec, int64_t scale) {
        lvec2 prod;
        prod.x = vec.x * scale;
        prod.y = vec.y * scale;
        return prod;
}
bool ivec2Eq(ivec2 a, ivec2 b) {
        return ((a.x == b.x) && (a.y == b.y));
}
bool lvec2Eq(lvec2 a, lvec2 b) {
        return ((a.x == b.x) && (a.y == b.y));
}
int32_t ivec2area(ivec2 v) {
        return v.x * v.y;
}
int64_t lvec2area(lvec2 v) {
        return v.x * v.y;
}

/************************ 3D ************************/
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
ivec3 ivec3Sub(ivec3 a, ivec3 b) {
        ivec3 diff;
        diff.x = a.x - b.x;
        diff.y = a.y - b.y;
        diff.z = a.z - b.z;
        return diff;
}
lvec3 lvec3Sub(lvec3 a, lvec3 b) {
        lvec3 diff;
        diff.x = a.x - b.x;
        diff.y = a.y - b.y;
        diff.z = a.z - b.z;
        return diff;
}
ivec3 ivec3Scale(ivec3 vec, int32_t scale) {
        ivec3 prod;
        prod.x = vec.x * scale;
        prod.y = vec.y * scale;
        prod.z = vec.z * scale;
        return prod;
}
lvec3 lvec3Scale(lvec3 vec, int64_t scale) {
        lvec3 prod;
        prod.x = vec.x * scale;
        prod.y = vec.y * scale;
        prod.z = vec.z * scale;
        return prod;
}
bool ivec3Eq(ivec3 a, ivec3 b) {
        return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}
bool lvec3Eq(lvec3 a, lvec3 b) {
        return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

/************************ 4D ************************/
ivec4 ivec4Add(ivec4 a, ivec4 b) {
        ivec4 sum;
        sum.x = a.x + b.x;
        sum.y = a.y + b.y;
        sum.z = a.z + b.z;
        sum.w = a.w + b.w;
        return sum;
}
lvec4 lvec4Add(lvec4 a, lvec4 b) {
        lvec4 sum;
        sum.x = a.x + b.x;
        sum.y = a.y + b.y;
        sum.z = a.z + b.z;
        sum.w = a.w + b.w;
        return sum;
}
ivec4 ivec4Sub(ivec4 a, ivec4 b) {
        ivec4 diff;
        diff.x = a.x - b.x;
        diff.y = a.y - b.y;
        diff.z = a.z - b.z;
        diff.w = a.w - b.w;
        return diff;
}
lvec4 lvec4Sub(lvec4 a, lvec4 b) {
        lvec4 diff;
        diff.x = a.x - b.x;
        diff.y = a.y - b.y;
        diff.z = a.z - b.z;
        diff.w = a.w - b.w;
        return diff;
}
ivec4 ivec4Scale(ivec4 vec, int32_t scale) {
        ivec4 prod;
        prod.x = vec.x * scale;
        prod.y = vec.y * scale;
        prod.z = vec.z * scale;
        prod.w = vec.w * scale;
        return prod;
}
lvec4 lvec4Scale(lvec4 vec, int64_t scale) {
        lvec4 prod;
        prod.x = vec.x * scale;
        prod.y = vec.y * scale;
        prod.z = vec.z * scale;
        prod.w = vec.w * scale;
        return prod;
}
bool ivec4Eq(ivec4 a, ivec4 b) {
        return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w));
}
bool lvec4Eq(lvec4 a, lvec4 b) {
        return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w));
}

