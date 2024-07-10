/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    int arr[10] = {0};
    int arrIndex = 0;
    for (int i = 0; i < 9; i++) {
        arr[arrIndex++] = i;
    }
    for (int i = 0; i < 10; i++) {
        printf("%d\n", arr[i]);
    }
    return 0;
}

