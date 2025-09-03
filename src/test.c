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
#include "lib/tllist.h"

struct test {
        int a;
        int b;
};

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        struct test a = {100, 200};
        struct test b = a;
        b.a = 300;
        printf("%d, %d\n", a.a, b.a);
}

