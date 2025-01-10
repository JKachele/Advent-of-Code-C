/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdbool.h>

#define EPSILON 0.00001
bool floatIsInt(float f) {
        int i = (int)f;
        float diff = f - i;
        if (diff < 0)
                diff = -1 * diff;
        return diff < EPSILON;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        float f1 = 0.1;
        float f2 = 0.2;
        float f3 = f1 + f2;
        printf("%.16f + %.16f = %.16f\n", f1, f2, f3);
        printf("%d\n", floatIsInt(f3));
        float f4 = 0.4;
        float f5 = 0.6;
        float f6 = f4 + f5;
        printf("%d\n", floatIsInt(f6));

        return 0;
}

