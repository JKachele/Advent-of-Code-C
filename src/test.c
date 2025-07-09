/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>

void testFun(int n, int *array2dIn) {
        int (*array2d)[n] = (int (*)[n])array2dIn;
        for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                        array2d[i][j] = (i * j) % 10;
                }
        }
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int n = 7;
        int array2d[n][n];
        int *arrPtr = (int *)array2d;
        for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                        array2d[i][j] = 0;
                }
        }

        for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                        printf("%d ", array2d[i][j]);
                }
                printf("\n");
        }

        testFun(n, arrPtr);

        printf("\n");
        for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                        printf("%d ", array2d[i][j]);
                }
                printf("\n");
        }

        return 0;
}

