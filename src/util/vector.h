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
#include <stdint.h>

typedef union ivec2 {
        int32_t raw[2];
        struct {
                int32_t x;
                int32_t y;
        };
} ivec2;

typedef union ivec3 {
        int32_t raw[3];
        struct {
                int32_t x;
                int32_t y;
                int32_t z;
        };
} ivec3;

typedef union ivec4 {
        int32_t raw[4];
        struct {
                int32_t x;
                int32_t y;
                int32_t z;
                int32_t w;
        };
} ivec4;

typedef union lvec2 {
        int64_t raw[2];
        struct {
                int64_t x;
                int64_t y;
        };
} lvec2;

typedef union lvec3 {
        int64_t raw[3];
        struct {
                int64_t x;
                int64_t y;
                int64_t z;
        };
} lvec3;

typedef union lvec4 {
        int64_t raw[4];
        struct {
                int64_t x;
                int64_t y;
                int64_t z;
                int64_t w;
        };
} lvec4;

typedef union vec2 {
        double raw[2];
        struct {
                double x;
                double y;
        };
} vec2;

typedef union vec3 {
        double raw[3];
        struct {
                double x;
                double y;
                double z;
        };
} vec3;

typedef union vec4 {
        double raw[4];
        struct {
                double x;
                double y;
                double z;
                double w;
        };
} vec4;

ivec2 ivec2Add(ivec2 a, ivec2 b);
lvec2 lvec2Add(lvec2 a, lvec2 b);
ivec2 ivec2Sub(ivec2 a, ivec2 b);
ivec3 ivec3Add(ivec3 a, ivec3 b);
lvec3 lvec3Add(lvec3 a, lvec3 b);
lvec3 lvec3Sub(lvec3 a, lvec3 b);
bool ivec2Eq(ivec2 a, ivec2 b);
bool lvec2Eq(lvec2 a, lvec2 b);
bool ivec3Eq(ivec3 a, ivec3 b);
bool lvec3Eq(lvec3 a, lvec3 b);
int32_t ivec2area(ivec2 v);
int64_t lvec2area(lvec2 v);

#endif

