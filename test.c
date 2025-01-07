/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdbool.h>

int numDigits(int i) {
        int digits = 0;
        while (i > 0) {
                digits++;
                i = i / 10;
        }
        return digits;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        printf("%d - %d\n", 19162064152, numDigits(12345));

        return 0;
}

