/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "src/util/linkedlist.h"

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    llist *ll = llist_create();
    for (int i = 0; i < 5; i++) {
        int *ip = malloc(sizeof(int));
        *ip = i;
        llist_add(ll, ip);
    }
    llist_print(ll, printInt);
    llist_free(ll);
    return 0;
}

