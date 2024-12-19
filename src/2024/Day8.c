/*************************************************
 *File----------Day8.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Dec 19, 2024 15:59:52 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

int MAX_X = 0;
int MAX_Y = 0;

typedef struct {
        char item;
        bool node;
} cell;

void part1(llist *ll) {
        MAX_X = strlen((char*)ll->head->data);
        MAX_Y = ll->length;
        cell grid[MAX_Y][MAX_X];

        llNode *current = ll->head;
        int lineNum = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < MAX_X; x++) {
                        grid[lineNum][x].item = str[x];
                }
                current = current->next;
                lineNum++;
        }
        /* for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        printf("%c", grid[y][x].item);
                }
                printf("\n");
        } */

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
        // llist *ll = getInputFile("assets/2024/Day8.txt");
        llist *ll = getInputFile("assets/test.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
