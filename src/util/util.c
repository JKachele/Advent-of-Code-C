/*************************************************
 *File----------util.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday May 08, 2024 10:51:16 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "util.h"
#include <stdio.h>

void printInt(void *data) {
        if (data == NULL)
                return;
        int *num = (int*)data;
        printf("%d", *num);
}

void printLong(void *data) {
        if (data == NULL)
                return;
        long *num = (long*)data;
        printf("%ld", *num);
}

ivec2 addIVec2(ivec2 a, ivec2 b) {
        ivec2 sum;
        sum.x = a.x + b.x;
        sum.y = a.y + b.y;
        return sum;
}

lvec2 addLVec2(lvec2 a, lvec2 b) {
        lvec2 sum;
        sum.x = a.x + b.x;
        sum.y = a.y + b.y;
        return sum;
}

bool ivec2Eq(ivec2 a, ivec2 b) {
        return ((a.x == b.x) && (a.y == b.y));
}

bool lvec2Eq(lvec2 a, lvec2 b) {
        return ((a.x == b.x) && (a.y == b.y));
}

int64_t ipow(int64_t base, uint8_t exp) {
        int result = 1;
        for(;;) {
                if (exp & 1)
                        result *= base;
                exp >>= 1;
                if(!exp)
                        break;
                base *= base;
        }
        return result;
}

// Credit to github.com/orlp 
// int64_t ipow(int64_t base, uint8_t exp) {
//     static const uint8_t highest_bit_set[] = {
//         0, 1, 2, 2, 3, 3, 3, 3,
//         4, 4, 4, 4, 4, 4, 4, 4,
//         5, 5, 5, 5, 5, 5, 5, 5,
//         5, 5, 5, 5, 5, 5, 5, 5,
//         6, 6, 6, 6, 6, 6, 6, 6,
//         6, 6, 6, 6, 6, 6, 6, 6,
//         6, 6, 6, 6, 6, 6, 6, 6,
//         6, 6, 6, 6, 6, 6, 6, 255, // anything past 63 is a guaranteed overflow with base > 1
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//         255, 255, 255, 255, 255, 255, 255, 255,
//     };
//
//     int64_t result = 1;
//
//     switch (highest_bit_set[exp]) {
//     case 255: // we use 255 as an overflow marker and return 0 on overflow/underflow
//         if (base == 1) {
//             return 1;
//         }
//         
//         if (base == -1) {
//             return 1 - 2 * (exp & 1);
//         }
//         
//         return 0;
//     case 6:
//         if (exp & 1) result *= base;
//         exp >>= 1;
//         base *= base;
//     case 5:
//         if (exp & 1) result *= base;
//         exp >>= 1;
//         base *= base;
//     case 4:
//         if (exp & 1) result *= base;
//         exp >>= 1;
//         base *= base;
//     case 3:
//         if (exp & 1) result *= base;
//         exp >>= 1;
//         base *= base;
//     case 2:
//         if (exp & 1) result *= base;
//         exp >>= 1;
//         base *= base;
//     case 1:
//         if (exp & 1) result *= base;
//     default:
//         return result;
//     }
// }
