/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Jul 10, 2024 13:25:23 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

void part1(llist *ll) {
    llNode *current = ll->head;
    int arrangementSum = 0;
    while(current != NULL) {
        char *str = (char*)current->data;

        char *cfg = strtok(str, " ");
        char *cfgNums = strtok(NULL, " ");
        printf("[%s] [%s]\n", cfg, cfgNums);

        llist *nums = llist_create();
        char* numStr = strtok(cfgNums, ",");
        while (numStr != NULL) {
            int *num = malloc(sizeof(int));
            *num = strtol(numStr, NULL, 10);
            llist_add(nums, num);
            numStr = strtok(NULL, ",");
        }
        // llist_print(nums, printInt);

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


