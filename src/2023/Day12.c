/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Jul 10, 2024 13:25:23 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

void part1(llist *ll) {
    llNode *current = ll->head;
    int arrangementSum = 0;
    while(current != NULL) {
        char *str = (char*)current->data;
        printf("%s\n", str);

        current = current->next;
    }

    printf("\nPart 1: Arrangement Sum: %d\n", arrangementSum);
}

void part2(llist *ll) {
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("\nPart 2: \n");
}

int main(int argc, char *argv[]) {
    // llist *ll = getInputFile("assets/2023/Day12.txt");
    llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll, printInput);

    part1(ll);
    part2(ll);

    return 0;
}


