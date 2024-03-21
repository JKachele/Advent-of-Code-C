/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    char str[256] = " 123 blue";
    char *out;
    long ret = strtol(str, &out, 10);
    printf("Split into [%ld] and [%s]\n", ret, out);
    return 0;
}

