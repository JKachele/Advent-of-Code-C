/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
// #include <stdbool.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>
// #include "src/util/util.h"
#include "src/lib/tllist.h"

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        tll(int) list = tll_init();

        int i = tll_back(list);
        printf("%d\n", i);

        return 0;
}

