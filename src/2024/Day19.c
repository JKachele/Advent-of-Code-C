/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Feb 10, 2025 09:49:20 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
// #include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
// #include "../lib/tllist.h"
// #include "../util/util.h"

void part1(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 1: \n\n");
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                // ll = getInputFile("assets/test.txt");
                ll = getInputFile("assets/tests/2024/Day19.txt");
        } else {
                ll = getInputFile("assets/2024/Day19.txt");
        }
        llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
