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
#include "util/vector.h"

uint8 hash(const char *str) {
        uint32 c = 0;
        uint32 hash = 0;
        while ((c = *str++)!= '\0') {
                printf("%c\n", c);
        }
        return (uint8)hash;
}


int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        hash("Hello, World!");
}

