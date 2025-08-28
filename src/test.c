/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "util/util.h"
#include "lib/tllist.h"

typedef tll(int) tllint;

void add(tllint *list, int val) {
        tll_push_back(*list, val);
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        tllint list = tll_init();
        tll_push_back(list, 1);
        tll_push_back(list, 2);
        tll_push_back(list, 3);

        tll_foreach(list, it) {
                printf("%d ", it->item);
        }
        printf("\n");

        add(&list, 4);
        add(&list, 5);
        add(&list, 6);

        tll_foreach(list, it) {
                printf("%d ", it->item);
        }
        printf("\n");
}

