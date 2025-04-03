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
#include "src/util/util.h"
// #include "src/lib/tllist.h"

void test(ivec2 *a) {
        ivec2 b = {654, 321};
        *a = b;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        ivec2 a = {123, 456};
        test(&a);
        printf("%d, %d\n", a.x, a.y);

        return 0;
}

