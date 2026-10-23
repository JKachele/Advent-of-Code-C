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
        struct {
                int32_t a;
                int32_t b;
        };
} ivec2;

typedef union ivec3 {
        int32_t raw[3];
        struct {
                int32_t x;
                int32_t y;
                int32_t z;
        };
        struct {
                int32_t a;
                int32_t b;
                int32_t c;
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
        struct {
                int32_t a;
                int32_t b;
                int32_t c;
                int32_t d;
        };
        struct {
                ivec2 v1;
                ivec2 v2;
        };
} ivec4;

typedef union lvec2 {
        int64_t raw[2];
        struct {
                int64_t x;
                int64_t y;
        };
        struct {
                int64_t a;
                int64_t b;
        };
} lvec2;

typedef union lvec3 {
        int64_t raw[3];
        struct {
                int64_t x;
                int64_t y;
                int64_t z;
        };
        struct {
                int64_t a;
                int64_t b;
                int64_t c;
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
        struct {
                int64_t a;
                int64_t b;
                int64_t c;
                int64_t d;
        };
        struct {
                lvec2 v1;
                lvec2 v2;
        };
} lvec4;

typedef union vec2 {
        double raw[2];
        struct {
                double x;
                double y;
        };
        struct {
                double a;
                double b;
        };
} vec2;

typedef union vec3 {
        double raw[3];
        struct {
                double x;
                double y;
                double z;
        };
        struct {
                double a;
                double b;
                double c;
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
        struct {
                double a;
                double b;
                double c;
                double d;
        };
        struct {
                vec2 v1;
                vec2 v2;
        };
} vec4;

ivec2 ivec2Add(ivec2 a, ivec2 b);
lvec2 lvec2Add(lvec2 a, lvec2 b);
ivec2 ivec2Sub(ivec2 a, ivec2 b);
lvec2 lvec2Sub(lvec2 a, lvec2 b);
ivec2 ivec2Scale(ivec2 vec, int32_t scale);
lvec2 lvec2Scale(lvec2 vec, int64_t scale);
bool ivec2Eq(ivec2 a, ivec2 b);
bool lvec2Eq(lvec2 a, lvec2 b);
int32_t ivec2area(ivec2 v);
int64_t lvec2area(lvec2 v);

ivec3 ivec3Add(ivec3 a, ivec3 b);
lvec3 lvec3Add(lvec3 a, lvec3 b);
ivec3 ivec3Sub(ivec3 a, ivec3 b);
lvec3 lvec3Sub(lvec3 a, lvec3 b);
ivec3 ivec3Scale(ivec3 vec, int32_t scale);
lvec3 lvec3Scale(lvec3 vec, int64_t scale);
bool ivec3Eq(ivec3 a, ivec3 b);
bool lvec3Eq(lvec3 a, lvec3 b);

ivec4 ivec4Add(ivec4 a, ivec4 b);
lvec4 lvec4Add(lvec4 a, lvec4 b);
ivec4 ivec4Sub(ivec4 a, ivec4 b);
lvec4 lvec4Sub(lvec4 a, lvec4 b);
ivec4 ivec4Scale(ivec4 vec, int32_t scale);
lvec4 lvec4Scale(lvec4 vec, int64_t scale);
bool ivec4Eq(ivec4 a, ivec4 b);
bool lvec4Eq(lvec4 a, lvec4 b);

#endif

