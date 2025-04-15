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

void test(ivec2 vecs[], int num) {
        for (int i = 0; i < num; i++) {
                vecs[i].x = i + 10;
                vecs[i].y = i + 10;
        }
}

void test2(ivec2 *a, ivec2 *b) {
        *b = *a;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        ivec2 vecs[10];
        for (int i = 0; i < 10; i++)
                vecs[i] = (ivec2){.x = i, .y = i};

        for (int i = 0; i < 10; i++)
                printf("[%d, %d] ", vecs[i].x, vecs[i].y);
        printf("\n");

        test(vecs, 10);

        for (int i = 0; i < 10; i++)
                printf("[%d, %d] ", vecs[i].x, vecs[i].y);
        printf("\n");

        ivec2 a = {.x = 1, .y = 1};
        ivec2 b = {.x = 2, .y = 2};
        test2(&a, &b);
        printf("[%d, %d]\n", b.x, b.y);

        return 0;
}

