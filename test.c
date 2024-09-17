/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>

void print(int test[]) {
    printf("%d\n", test[0]);
}

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    int test[5];
    for (int i = 0; i < 5; i++)
        test[i] = i;
    print(test);
    print(test + 1);
    return 0;
}

