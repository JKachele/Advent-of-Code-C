/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "src/util/util.h"


int main(int argc, char *argv[]) {
        printf("Hello, World!\n");
        int reg[3] = {0};
        for (int i = 0; i < 64; i++) {
                reg[0] = i;
                reg[1] = reg[0] % 8;
                reg[1] = reg[1] ^ 3;
                reg[2] = reg[0] / ipow(2, reg[1]);
                reg[1] = reg[1] ^ reg[2];
                reg[1] = reg[1] ^ 5;
                printf("A: %d, Out: %d\n", i, reg[1] % 8);
        }

        return 0;
}

