/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "src/util/linkedlist.h"
#include "src/util/util.h"

int func(int a) {
        int *ap = malloc(sizeof(int));
        *ap = a;

        llist *ll = llist_create();
        llist_add(ll, ap);
        for (int i = 0; i < 10; i++) {
                int *j = malloc(sizeof(int));
                *j = i;
                llist_add(ll, j);
        }
        llist_print(ll, printInt);

        llNode *cur = ll->head;
        llNode *three;
        while (cur != NULL) {
                int i = *(int*)cur->data;
                if (i == 3)
                        three = cur;
                cur = cur->next;
        }

        int i3 = *(int*)three->data;
        printf("Three: %d\n", i3);

        return 0;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        func(10);

        return 0;
}

