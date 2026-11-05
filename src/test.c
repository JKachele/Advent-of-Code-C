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
#include "util/talist.h"

typedef tal(int) talint;

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        talint list = tal_init();

        for (int i = 0; i < 20; i++) {
                tal_add(list, i);
        }

        tal_for(list, i) {
                int n = list.array[i];
                if (n % 3 == 0) {
                        tal_remove(list, i);
                        i--;
                        continue;
                }
                printf("%d\n", n);
        }

        return 0;
}

