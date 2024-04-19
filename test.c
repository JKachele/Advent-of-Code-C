/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "src/util/linkedlist.h"

void printInt(void* data) {
    if (data == NULL) {return;}
    int *num = (int*)data;
    printf("%d", *num);
}

int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    llist *ll = llist_create();
    int *int1 = malloc(sizeof(int));
    int *int2 = malloc(sizeof(int));
    int *int3 = malloc(sizeof(int));
    *int1 = 1;
    *int2 = 2;
    *int3 = 3;
    llist_add(ll, int1);
    llist_add(ll, int2);
    llist_add(ll, int3);
    printf("length = %d\n", ll->length);
    llist_print(ll, printInt);
    llist_remove_node(ll, ll->head);
    printf("length = %d\n", ll->length);
    llist_print(ll, printInt);

    return 0;
}

