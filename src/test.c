/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/talist.h"

typedef tal(int) talint;

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        talint l = tal_init();
        printf("%lu\n", l.capicity);
        for (int i = 0; i < 32; i++) {
                tal_add(l, i);
        }
        printf("%lu\n", l.capicity);

        for (int i = 0; i < (int)l.length; i++) {
                printf("%d ", l.array[i]);
        }
        printf("\n");

        tal_insert(l, 20, 10000);

        for (int i = 0; i < (int)l.length; i++) {
                printf("%d ", tal_get(l, i));
        }
        printf("\n");
        printf("%lu\n", l.capicity);

        printf("%d\n", tal_pop_front(l));
        for (int i = 0; i < (int)l.length; i++) {
                printf("%d ", l.array[i]);
        }
        printf("\n");

        tal_remove(l, 19);
        for (int i = 0; i < (int)l.length; i++) {
                printf("%d ", l.array[i]);
        }
        printf("\n");
}

