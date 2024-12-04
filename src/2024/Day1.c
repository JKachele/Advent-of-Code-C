/*************************************************
 *File----------Day1.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 02, 2024 08:47:32 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

void part1(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                current = current->next;
        }
        printf("\nPart 1: \n");
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                current = current->next;
        }
        printf("\nPart 2: \n");
}

int main(int argc, char *argv[]) {
        llist *ll = getInputFile("assets/2024/Day1.txt");
        // llist *ll = getInputFile("assets/test.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


