/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util/util.h"

void testFn(char *str) {
        char *tok = strtok(str, " ");
        while (tok != NULL) {
                printf("%s\n", tok);
                tok = strtok(NULL, " ");
        }
        printf("------------------\n");
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        char test[] = "kjas kbjlaks lkjsbdvl, skjdvblaj laksdvljb, lkjasbvl lkasjdv";

        char *tok = strtok(test, ",");
        printf("%s\n", tok);
        testFn(tok);
        // tok = strtok(NULL, ",");
        // printf("%s\n", tok);
        // testFn(tok);

        return 0;
}

