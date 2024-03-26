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
    llNode *node1 = llist_create_node("Hello 1");
    llNode *node2 = llist_create_node("Hello 2");
    llNode *node3 = llist_create_node("Hello 3");
    llNode *node4 = llist_create_node("Hello 4");

    llist_add_node(ll, node1);
    llist_add_node(ll, node2);
    llist_add_node(ll, node3);
    llist_add_node(ll, node4);

    llist_print(ll);

    return 0;
}

