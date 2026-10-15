/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util/util.h"
#include "util/md5.h"

u16 regHash(const char *name) {
        u16 hash = 0;
        for (u64 i = 0; i < strlen(name); i++) {
                hash *= 27;
                hash += (name[i] - 'a') + 1;
        }
        return hash;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        char *hello = "Hello, World!";

        uint8_t result[16];
        md5(hello, result);

        for (int i = 0; i < 16; i++) {
                printf("%02x", result[i]);
        }
        printf("\n");

        uint64_t hash = md5_64(hello);
        printf("                %016lx\n", hash);

        printf("%u\n", regHash("a"));
        printf("%u\n", regHash("aa"));
        printf("%u\n", regHash("aaa"));

        return 0;
}

