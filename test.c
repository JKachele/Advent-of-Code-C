/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    int x = 5;
    bool test[x];
    memset(test, 0, x);
    for (int i = 0; i < x; i++) {
        printf("%d\n", test[i]);
    }
    char array2D[10][10];
    printf("Size: %lu\n", sizeof(array2D));
    return 0;
}

