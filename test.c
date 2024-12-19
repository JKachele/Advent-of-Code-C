/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>

long concat(long a, long b) {
        long pow = 10;
        while (b >= pow)
                pow *= 10;
        return a * pow + b;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");
        printf("123 || 456 -> %ld\n", concat(123, 456));
        return 0;
}

