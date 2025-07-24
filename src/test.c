/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "lib/tllist.h"

typedef tll(int) tllint;

void tllPrint(tllint tllist) {
        tll_foreach(tllist, it) {
                printf("%d ", it->item);
        }
        printf("\n");
}

void tllintCopy(tllint *dest, tllint src) {
        tll_foreach(src, it) {
                tll_push_back(*dest, it->item);
        }
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        tllint tll1 = tll_init();
        tll_push_back(tll1, 0);
        tll_push_back(tll1, 1);
        tll_push_back(tll1, 2);
        tll_push_back(tll1, 3);
        tll_push_back(tll1, 4);
        tllPrint(tll1);

        tllint tll2 = tll_init();
        tllintCopy(&tll2, tll1);
        tllPrint(tll2);

        tll_push_back(tll2, 5);
        tll_push_back(tll2, 6);
        tll_push_back(tll2, 7);
        tll_push_back(tll2, 8);
        tllPrint(tll1);
        tllPrint(tll2);

        return 0;
}

