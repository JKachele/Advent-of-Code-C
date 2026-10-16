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

        char *test = "12/34";
        char *end;
        int testNum = strtol(test, &end, 10);
        int testNum1 = strtol(end+1, NULL, 10);
        printf("%d, %d\n", testNum, testNum1);

        return 0;
}

