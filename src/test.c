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
#include "util/util.h"
#include "util/vector.h"

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int arr[10] = {0};

        int index = 20;
        for (int i=0; i<10; i++) {
                arr[i] = index++;
        }

        for (int i=0; i<10; i++) {
                printf("%d\n", arr[i]);
        }
}

