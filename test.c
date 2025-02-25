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
// #include <string.h>
// #include "src/util/util.h"
#include "src/lib/tllist.h"

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        tll(int) arr = tll_init();
        tll_push_back(arr, 1);
        tll_push_back(arr, 2);
        tll_push_back(arr, 4);
        tll_push_back(arr, 8);

        tll_foreach(arr, it) {
                printf("%d ", it->item);
        }
        printf("\n");

        return 0;
}

