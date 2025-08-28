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

void tilt(ivec2 size, int arr[size.y][size.x], int8 dir) {
        bool vert = (dir == 0 || dir == 2);
        bool invert = (dir == 2 || dir == 3);

        int sizeI = vert ? size.x : size.y;
        int sizeJ = vert ? size.y : size.x;

        for (int i=0; i<sizeI; i++) {
                for (int j=0; j<sizeJ; j++) {
                        int jInv = invert ? (sizeJ - 1) - j : j;

                        int x = vert ? i : jInv;
                        int y = vert ? jInv : i;
                        printf("%d ", arr[y][x]);
                }
                // printf("\n");
        }
        printf("\n");
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int arr[4][3] = {
                {0, 1,  2},
                {3, 4,  5},
                {6, 7,  8},
                {9, 10, 11}
        };

        ivec2 size = {3, 4};

        printf("UP: ");
        tilt(size, arr, 0);
        printf("LEFT: ");
        tilt(size, arr, 1);
        printf("DOWN: ");
        tilt(size, arr, 2);
        printf("RIGHT: ");
        tilt(size, arr, 3);
}

