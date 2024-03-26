/*************************************************
 *File----------Day3.c
 *Project-------{{PROJECT}}
 *Author--------Justin Kachele
 *Created-------Sunday Mar 24, 2024 18:55:44 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

void part1(llist *ll) {
    llNode *current = ll->head;
    int maxX = llist_get_longest_node(ll);
    int maxY = ll->length;
    char schematic[maxX][maxY];
    for(int y = 0; current != NULL; y++) {
        char str[BUFFER_SIZE];
        strncpy(str, current->str, BUFFER_SIZE);
        for (int x = 0; x < strlen(str); x++) {
            schematic[x][y] = str[x];
        }
        current = current->next;
    }

    int partNumSum = 0;
    bool numberFound = false;
    bool isPartNum = false;
    int currentNum = 0;
    for (int y = 0; y < maxY; y++) {
        for (int x = 0; x < maxX; x++) {
            // printf("[%c] ", schematic[x][y]);
            if (schematic[x][y] >= '0' && schematic[x][y] <= '9') {
                if (numberFound) {
                    currentNum = (currentNum * 10) + (schematic[x][y] - '0');
                } else {
                    currentNum = schematic[x][y] = '0';
                    numberFound = true;
                }
                // check for adjacent symbols
                if (isPartNum) {
                    continue;
                }
                for (int yMod = y - 1; yMod <= y + 1; yMod++) {
                    for (int xMod = x - 1; xMod <= x + 1; xMod++) {
                        if (yMod < 0 || yMod >= maxY ||
                            xMod < 0 || xMod >= maxX) {
                            continue;
                        }
                        char c = schematic[xMod][yMod];
                        if ((c <= 0 || c >= '9') && c != '.') {
                            isPartNum = true;
                            break;
                        }
                    }
                }
            }
        }
        // printf("\n");
    }
    printf("Part 1: \n");
}

void part2(llist *ll) {
    llNode *current = ll->head;
    while(current != NULL) {
        current = current->next;
    }
    printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
    // llist ll = getInputFile("assets/2023/Day3.txt");
    llist *ll = getInputFile("assets/test.txt");
    llist_print(ll);

    part1(ll);
    part2(ll);

    return 0;
}

