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
#include "util/quicksort.h"

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int arr[] = {12, 3, 6, 7, 4, 19, 15};
        int med = quickselect(arr, 0, 6, 3);
        printf("%d\n", med);
}

