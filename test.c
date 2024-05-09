/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>

long gcd(long x, long y) {
    if (x == 0) {
        return y;
    } else {
        return gcd(y % x, x);
    }
}

long lcm(long x, long y) {
    return ((x * y) / gcd(x, y));
}

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    printf("Least common multiple of 23 and 76 is %ld\n", lcm(23, 76));
    return 0;
}

