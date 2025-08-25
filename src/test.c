/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "util/util.h"

typedef struct {
        ivec2 pos;
        int32 time;
} state;

int32 getIndex(ivec2 size, int32 lcm, state s) {
        return ((((s.pos.y - 1) * (size.x - 2)) + (s.pos.x - 1)) * lcm) +
                (s.time % lcm);
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        ivec2 size = {8, 6};
        state s = {{1, 0}, 1};
        printf("%d\n", getIndex(size, 12, s));

        return 0;
}

