/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "util/util.h"
#include "lib/tllist.h"

typedef tll(int) tllint;

// FNV Hashing Algorithm - H is seed
uint32 FNV(const char *key, uint32 h) {
        h ^= 2166136261UL;
        const uint8 *data = (const uint8*)key;
        for (int i=0; data[i]!='\0'; i++) {
                h ^= data[i];
                h *= 16777619;
        }
        return h;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        printf("%u\n", (uint16)FNV("A", 0x12345678));
        printf("%u\n", (uint16)FNV("R", 0x12345678));

}

