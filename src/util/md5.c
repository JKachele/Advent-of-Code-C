/*************************************************
 *File----------md5.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Oct 14, 2026 10:57:02 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "md5.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define A 0x67452301U
#define B 0xefcdab89U
#define C 0x98badcfeU
#define D 0x10325476U

typedef uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;

static const u32 S[64] = {7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
                          5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
                          4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
                          6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

static const u32 K[64] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                          0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                          0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                          0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                          0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                          0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                          0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                          0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                          0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                          0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                          0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                          0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                          0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                          0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                          0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                          0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

static u32 rotate_left(u32 x, u32 s) {
        s = s % 32;
        if (s == 0) return x;
        return (x << s) | (x >> (32 - s));
}

static u32 F(u32 b, u32 c, u32 d, u32 i) {
        if (i < 16) {
                return (b & c) | ((~b) & d);
        } else if (i < 32) {
                return (b & d) | ((~d) & c);
        } else if (i < 48) {
                return b ^ c ^ d;
        } else {
                return c ^ ( b | (~d));
        }
}

static u32 input_index(u32 i) {
        if (i < 16) {
                return i;
        } else if (i < 32) {
                return ((5 * i) + 1) % 16;
        } else if (i < 48) {
                return ((3 * i) + 5) % 16;
        } else {
                return (7 * i) % 16;
        }
}

static void md5_step(const u32 input[16], u32 buffer[4]) {
        u32 a = buffer[0];
        u32 b = buffer[1];
        u32 c = buffer[2];
        u32 d = buffer[3];

        for (u32 i = 0; i < 64; i++) {
                u32 cc = b;
                u32 dd = c;
                u32 aa = d;
                u32 bb = F(b, c, d, i);
                bb += a;
                bb += input[input_index(i)];
                bb += K[i];
                bb = rotate_left(bb, S[i]);
                bb += b;

                a = aa;
                b = bb;
                c = cc;
                d = dd;
        }

        buffer[0] += a;
        buffer[1] += b;
        buffer[2] += c;
        buffer[3] += d;
}

static void md5_update(const u8 input[64], u32 buffer[4]) {
        u32 md5_input[16];
        for (u32 j = 0; j < 16; j++) {
                md5_input[j] = (u32)(input[(j * 4) + 3]) << 24 |
                        (u32)(input[(j * 4) + 2]) << 16 |
                        (u32)(input[(j * 4) + 1]) <<  8 |
                        (u32)(input[(j * 4)]);
        }
        md5_step(md5_input, buffer);
}

void md5(const char *input, u8 result[16]) {
        u32 buffer[4] = {A, B, C, D};
        u64 input_len = strlen(input);

        u32 offset = 0;
        u8 input_buf[64];
        for (u32 i = 0; i < input_len; i++) {
                input_buf[offset++] = (u8)*(input + i);

                if (offset % 64 == 0) {
                        md5_update(input_buf, buffer);
                        offset = 0;
                }
        }

        // Pad remaining bits
        if (offset < 56) {
                // Can finish hash with 1 more itteration
                input_buf[offset++] = 0x80;
                while (offset < 56) {
                        input_buf[offset++] = 0x00;
                }
                // Add input length to last 8 bytes in little-endian
                for (u32 i = 0; i < 8; i++) {
                        input_buf[offset++] = (u8)((input_len * 8) >> (8 * i));
                }
                md5_update(input_buf, buffer);
        } else {
                // Need 2 itterations, 1 to pad final input, and 1 to add input length
                input_buf[offset++] = 0x80;
                while (offset < 64) {
                        input_buf[offset++] = 0x00;
                }
                md5_update(input_buf, buffer);

                // last itteration is all zeros with length in last 8 bytes
                for (u32 i = 0; i < 56; i++) {
                        input_buf[i] = 0x00;
                }
                // Add input length to last 8 bytes in little-endian
                for (u32 i = 0; i < 8; i++) {
                        input_buf[offset++] = (u8)((input_len * 8) >> (8 * i));
                }
                md5_update(input_buf, buffer);
        }

        // Convert from little-endian to result
        for (u32 i = 0; i < 4; i++) {
                result[(i * 4) + 0] = (u8)((buffer[i] & 0x000000FF));
                result[(i * 4) + 1] = (u8)((buffer[i] & 0x0000FF00) >>  8);
                result[(i * 4) + 2] = (u8)((buffer[i] & 0x00FF0000) >> 16);
                result[(i * 4) + 3] = (u8)((buffer[i] & 0xFF000000) >> 24);
        }
}

uint64_t md5_64(const char *input) {
        u8 result[16];
        md5(input, result);

        u64 hash = 0;
        for (u32 i = 0; i < 8; i++) {
                hash <<= 8;
                hash |= (u64)(result[i+8]);
        }
        return hash;
}

