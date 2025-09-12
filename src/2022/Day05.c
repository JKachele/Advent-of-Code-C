/*************************************************
 *File----------Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 24, 2025 14:49:11 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

typedef tll(char) tllchar;

typedef struct {
        int boxes;
        int from;
        int to;
} move;

typedef tll(move) tllmove;

void printStacks(tllchar stacks[], int numStacks) {
        for (int i = 0; i < numStacks; i++) {
                printf("%d: ", i);
                tll_foreach(stacks[i], it) {
                        printf("[%c] ", it->item);
                }
                printf("\n");
        }
}

void printMoves(tllmove moves) {
        tll_foreach(moves, it) {
                move m = it->item;
                printf("boxes: %d, from: %d, to: %d\n", m.boxes, m.from, m.to);
        }
}

void moveBoxes(move m, tllchar stacks[], int numStacks) {
        for (int i = 0; i < m.boxes; i++) {
                char box = tll_pop_back(stacks[m.from - 1]);
                tll_push_back(stacks[m.to - 1], box);
        }
}

void moveBoxes2(move m, tllchar stacks[], int numStacks) {
        tllchar tempStack = tll_init();
        for (int i = 0; i < m.boxes; i++) {
                char box = tll_pop_back(stacks[m.from - 1]);
                tll_push_back(tempStack, box);
                // tll_push_back(stacks[m.to - 1], box);
        }

        for (int i = 0; i < m.boxes; i++) {
                char box = tll_pop_back(tempStack);
                tll_push_back(stacks[m.to - 1], box);
        }
}

void part1(llist *ll) {
        int numStacks = (strlen((char*)ll->head->data) + 1) / 4;
        tllchar stacks[numStacks];
        for (int i = 0; i < numStacks; i++) {
                stacks[i] = (tllchar)tll_init();
        }

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (strlen(str) == 0) break;
                if (str[1] == '1') break;
                // printf("%s\n", str);

                for (int i = 0; i < numStacks; i++) {
                        int stackIndex = (i * 4) + 1;
                        if (str[stackIndex] == ' ') continue;
                        tll_push_front(stacks[i], str[stackIndex]);
                }

                current = current->next;
        }
        // printStacks(stacks, numStacks);

        tllmove moves = tll_init();
        current = current->next->next;
        while (current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                move m;
                strtok(str, " ");
                m.boxes = strtol(strtok(NULL," "), (char**)NULL, 10);
                strtok(NULL, " ");
                m.from = strtol(strtok(NULL," "), (char**)NULL, 10);
                strtok(NULL, " ");
                m.to = strtol(strtok(NULL,""), (char**)NULL, 10);
                tll_push_back(moves, m);

                current = current->next;
        }
        // printMoves(moves);
        
        tll_foreach(moves, it) {
                move m = it->item;
                moveBoxes(m, stacks, numStacks);
        }
        // printStacks(stacks, numStacks);

        char tops[numStacks + 1];
        for (int i = 0; i < numStacks; i++) {
                tops[i] = tll_back(stacks[i]);
        }
        tops[numStacks] = '\0';

        printf("Part 1: %s\n\n", tops);
}

void part2(llist *ll) {
        int numStacks = (strlen((char*)ll->head->data) + 1) / 4;
        tllchar stacks[numStacks];
        for (int i = 0; i < numStacks; i++) {
                stacks[i] = (tllchar)tll_init();
        }

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (strlen(str) == 0) break;
                if (str[1] == '1') break;
                // printf("%s\n", str);

                for (int i = 0; i < numStacks; i++) {
                        int stackIndex = (i * 4) + 1;
                        if (str[stackIndex] == ' ') continue;
                        tll_push_front(stacks[i], str[stackIndex]);
                }

                current = current->next;
        }
        // printStacks(stacks, numStacks);

        tllmove moves = tll_init();
        current = current->next->next;
        while (current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                move m;
                strtok(str, " ");
                m.boxes = strtol(strtok(NULL," "), (char**)NULL, 10);
                strtok(NULL, " ");
                m.from = strtol(strtok(NULL," "), (char**)NULL, 10);
                strtok(NULL, " ");
                m.to = strtol(strtok(NULL,""), (char**)NULL, 10);
                tll_push_back(moves, m);

                current = current->next;
        }
        // printMoves(moves);
        
        tll_foreach(moves, it) {
                move m = it->item;
                moveBoxes2(m, stacks, numStacks);
        }
        // printStacks(stacks, numStacks);

        char tops[numStacks + 1];
        for (int i = 0; i < numStacks; i++) {
                tops[i] = tll_back(stacks[i]);
        }
        tops[numStacks] = '\0';

        printf("Part 2: %s\n\n", tops);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day05.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day05.txt");
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

