/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "util/util.h"

int32 strtoid(const char *str) {
        int32 id = (int32)(str[0] - 'a') << 15;
        id += (int32)(str[1] - 'a') << 10;
        id += (int32)(str[2] - 'a') << 5;
        id += (int32)(str[3] - 'a');
        return id;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        printf("%d\n", strtoid("root"));

        return 0;
}

