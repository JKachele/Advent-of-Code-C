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

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        ivec2 pos = {-122, 134};

        int16 x = (int16)pos.x;
        int16 y = (int16)pos.y;
        printf("%d, %d\n", x, y);
        printf("%b, %b\n\n", x, y);
        uint16 xu = *((uint16*)&x);
        uint16 yu = *((uint16*)&y);
        printf("%u, %u\n", xu, yu);
        printf("%b, %b\n\n", xu, yu);
        uint32 index = (uint32)xu << 16 | yu;
        printf("%u\n", index);
        printf("%b\n", index);
}

