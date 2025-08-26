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

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int *test = calloc(10, sizeof(int));
        test[3] = 10;
        printf("%d\n", test[3]);
        printf("%d\n", *(test + 3));

        return 0;
}

