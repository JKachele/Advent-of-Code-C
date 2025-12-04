/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");
        // system("clear");
        // for (int i = 0; i < 10; i++) {
        //         printf("\033[H");       // Home
        //         for (int j = 0; j < 10 - i; j++) {
        //                 printf("%d: %d\n", j, i);
        //         }
        //         usleep(500000);
        // }

        for (int i = 1; i < 138; i++) {
                if (i == 1 || i == 137)
                        printf("********************\n");
                else
                        printf("%d\n", i);
        }

        return 0;
}

