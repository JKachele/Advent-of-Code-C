/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "src/lib/tllist.h"

int arrToInt(int arr[], int len) {
        int out = 0;
        int mult = 1;
        for (int i = len - 1; i >= 0; i--) {
                out += arr[i] * mult;
                mult *= 10;
        }
        return out;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int arr[] = {2, 4, 8};
        printf("%d\n", arrToInt(arr, 3));

        return 0;
}

