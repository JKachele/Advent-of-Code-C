/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "src/lib/tllist.h"

bool startsWith(const char* a, const char *b) {
        int len = strlen(a) < strlen(b) ? strlen(a) : strlen(b);
        for (int i = 0; i < len; i++) {
                if (a[i] != b[i])
                        return false;
        }
        return true;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        printf("%d\n", startsWith("Hello!!!", "He"));
        printf("%lu\n", strlen("\0"));

        return 0;
}

