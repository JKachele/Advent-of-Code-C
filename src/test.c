/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include "util/talist.h"

enum test {
        TEST1,
        TEST2,
        TEST3
};

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        enum test test = TEST1;
        printf("%d\n", test);
        test++;
        printf("%d\n", test);
        test++;
        printf("%d\n", test);
        test++;
        printf("%d\n", test);

        return 0;
}

