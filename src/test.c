/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util/talist.h"

typedef tal(int) talint;

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        talint a = tal_init();
        talint b = tal_init();
        for (int i = 0; i < 10; i++)
                tal_add(a, i);
        for (int i = 100; i < 200; i++)
                tal_add(b, i);

        tal_for(a, i)
                printf("%d ", a.array[i]);
        printf("\n");
        tal_for(b, i)
                printf("%d ", b.array[i]);
        printf("\n");

        printf("%lu\n", a.capicity);
        tal_append(a, b);
        printf("%lu\n", a.capicity);
        tal_for(a, i)
                printf("%d ", a.array[i]);
        printf("\n");

        return 0;
}

