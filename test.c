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

uint64 parseDir(const char *line) {
        return computeHash(line + 4);
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        char *line = "dir bduiw";
        char *dir = "bduiw";

        printf("%lu\n", parseDir(line));
        printf("%lu\n", computeHash(dir));

        return 0;
}

