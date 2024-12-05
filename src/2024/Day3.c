/*************************************************
 *File----------Day3.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Dec 04, 2024 17:18:52 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

char* regexp(char *str, char *patrn) {
        return "";
}

void part1(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("\nPart 1: \n");
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("\nPart 2: \n");
}

int main(int argc, char *argv[]) {
        llist *ll = getInputFile("assets/2024/Day3.txt");
        // llist *ll = getInputFile("assets/test.txt");
        llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
