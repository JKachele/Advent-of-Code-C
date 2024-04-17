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
    printf("Size of int = %lu\n", sizeof(int));
    printf("Size of long = %lu\n", sizeof(long));
    printf("Size of char = %lu\n", sizeof(char));

    printf("Max of 1 and 2: %d\n", max(1,2));
    printf("Max of 1 and 1: %d\n", max(1,1));
    printf("Min of 1 and 2: %d\n", min(1,2));
    printf("Min of 1 and 1: %d\n", min(1,1));

    return 0;
}

