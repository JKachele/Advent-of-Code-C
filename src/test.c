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

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        char *test = "(1276x3274)a";

        int i = 0;
        int len = 0;
        while (test[++i] != 'x') {
                len *= 10;
                len += test[i] - '0';
        }
        int times = 0;
        while (test[++i] != ')') {
                times *= 10;
                times += test[i] - '0';
        }
        printf("%d, %d - %c\n", len, times, test[i]);

        return 0;
}

