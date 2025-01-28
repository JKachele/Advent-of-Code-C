/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>


int main(int argc, char *argv[]) {
        printf("Hello, World!\n");
        int arr[4] = {0, 0, 0, 0};
        int *pI = &arr[2];
        *pI = 4;
        printf("%d\n", arr[2]);

        return 0;
}

