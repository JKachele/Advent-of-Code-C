/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>

typedef struct {
    int x;
    int y;
} i_vector2;

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    i_vector2 test = {123, 456};
    i_vector2 test2 = test;
    printf("Vector1: (%d, %d), Vector2: (%d, %d)\n", test.x, test.y, test2.x, test2.y);
    test2.x = 789;
    printf("Vector1: (%d, %d), Vector2: (%d, %d)\n", test.x, test.y, test2.x, test2.y);
    return 0;
}

