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

void printList(talint list) {
        printf("[ ");
        tal_for(list, i) {
                printf("%d ", list.array[i]);
        }
        printf("]\n");
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        talint list = tal_init();
        for (int i = 0; i < 10; i++) {
                tal_add(list, i);
        }
        printList(list);

        tal_add_unique(list, 7);
        printList(list);

        tal_add_unique(list, 14);
        printList(list);

        tal_add_unique(list, 4);
        printList(list);

        return 0;
}

