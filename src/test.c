/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/util.h"
#include "util/vector.h"

#define SEED 0x12345678

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

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        printf("%x\n", FNV("bdq", SEED));
        printf("%x\n", (u16)FNV("bdq", SEED));


}

