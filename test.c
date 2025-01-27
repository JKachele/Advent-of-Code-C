/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "src/util/util.h"


int main(int argc, char *argv[]) {
        printf("Hello, World!\n");
        ivec2 vec1 = {1, 2};
        ivec2 vec2 = vec1;
        printf("1:(%d, %d) ", vec1.x, vec1.y);
        printf("2:(%d, %d)\n", vec2.x, vec2.y);
        vec2.x = 30;
        printf("1:(%d, %d) ", vec1.x, vec1.y);
        printf("2:(%d, %d)\n", vec2.x, vec2.y);

        return 0;
}

