/*************************************************
 *File----------util.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday May 08, 2024 10:51:16 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isDigit(char c) {
        return (c >= '0' && c <= '9');
}

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

// Euclidean Algorithm to find GCD
int64 gcd(int64 n1, int64 n2) {
        int64 a = labs(n1);
        int64 b = labs(n2);

        // Everything divides zero
        if (a == 0)
                return b;
        if (b == 0)
                return a;

        // Base case
        if (a == b)
                return a;

        if (a > b)
                return gcd(a - b, b);
        else
                return gcd(a, b - a);
}

int64 lcm(int64 n1, int64 n2) {
        return (n1 / gcd(n1, n2)) * n2;
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

uint64 computeHash(char const *input) {
        const uint64 p = 31;
        const uint64 m = 1000000009;
        uint64 hashVal = 0;
        uint64 pow = 1;
        for (int i = 0; i < (int)strlen(input); i++) {
                hashVal = (hashVal + ((int)input[i] * pow)) % m;
                pow = (pow * p) % m;
        }
        return hashVal;
}

// FNV Hashing Algorithm - H is seed
u32 FNV(const char *key, u32 h) {
        h ^= 2166136261UL;
        const u8 *data = (const u8*)key;
        for (int i=0; data[i]!='\0'; i++) {
                h ^= data[i];
                h *= 16777619;
        }
        return h;
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
