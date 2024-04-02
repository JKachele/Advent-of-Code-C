/*************************************************
 *File----------Day5.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 02, 2024 15:26:20 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

// Part 1 {{{
void part1(llist *ll) {
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 1: \n");
}   // }}}

// Part 2 {{{
void part2(llist *ll) {
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 2: \n");
}   // }}}

int main(int argc, char *argv[]) {
    llist *ll = getInputFile("assets/2023/Day5.txt");
    // llist *ll = getInputFile("assets/test.txt");
    llist_print(ll);

    part1(ll);
    part2(ll);

    return 0;
}

