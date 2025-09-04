/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "util/util.h"
#include "lib/tllist.h"

int64 gcd(int64 a, int64 b) {
        while (b != 0) {
                int64 tmp = a;
                a = b;
                b = tmp % b;
        }
        return a;
}

int64 lcm(int64 a, int64 b) {
        return a * b / gcd(a, b);
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        int64 nums[] = {12, 15, 75};
        int64 ans = 1;
        for (int i=0; i<3; i++) {
                ans = lcm(ans, nums[i]);
        }
        printf("%ld\n", ans);
}

