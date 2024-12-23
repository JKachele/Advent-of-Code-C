/*************************************************
 *File----------Day9.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 23, 2024 16:07:17 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
// #include "../util/util.h"

// Using different File input because of input file format (One long line)
char *getInput(char *fileName) {
        FILE *inputFile = fopen(fileName, "r");     // Open file in read mode
        char *line = malloc(BUFFER_SIZE);
        size_t len = 0;
        ssize_t read;

        return NULL;
}

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
        llist *ll = getInputFile("assets/2024/Day9.txt");
        // llist *ll = getInputFile("assets/test.txt");
        llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


