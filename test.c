/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    char test[] = " 1 2 3 4 5   1 2 3 4 ";

    char *num = strtok(test, " ");
    while (num != NULL) {
        printf("**%s**\n", num);
        num = strtok(NULL, " ");
    }

    return 0;
}

