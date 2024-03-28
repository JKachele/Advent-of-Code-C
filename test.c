/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>

void printArray(int array[10]) {
    for (int i = 0; i < 10; i++) {
        printf("[%d] ", array[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    int array[10][10];
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            array[y][x] = (y * 10) + x;
        }
    }

    // for (int y = 0; y < 10; y++) {
    //     for (int x = 0; x < 10; x++) {
    //         printf("[%d] ", array[x][y]);
    //     }
    //     printf("\n");
    // }

    printArray(array[3]);

    return 0;
}

