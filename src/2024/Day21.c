/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Feb 13, 2025 09:41:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
// #include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
// #include "../util/util.h"
/*
 * +---+---+---+        +---+---+        +---+---+   
 * | 7 | 8 | 9 |        | ^ | A |        | ^ | A |   
 * +---+---+---+    +---+---+---+    +---+---+---+   
 * | 4 | 5 | 6 |    | < | v | > |    | < | v | > |   
 * +---+---+---+    +---+---+---+    +---+---+---+   
 * | 1 | 2 | 3 |                   
 * +---+---+---+                   
 *     | 0 | A |                   
 *     +---+---+                   
 */

#define NUM_CODES 5
#define CODE_LENGTH 3

uint64_t getComplexity(int code[CODE_LENGTH]) {
        return 0;
}

void part1(llist *ll) {
        int codes[NUM_CODES][CODE_LENGTH];

        llNode *current = ll->head;
        for (int i = 0; i < NUM_CODES; i++) {
                char str[BUFFER_SIZE + 1];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int j = 0; j < CODE_LENGTH; j++) {
                        codes[i][j] = str[j] - '0';
                }
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
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day21.txt");
        } else {
                ll = getInputFile("assets/2024/Day21.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}
