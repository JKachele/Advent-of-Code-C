/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 13, 2026 08:45:00 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/md5.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/talist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(int) talint;
typedef tal(u32) talu32;

struct program {
        char *name;
        u32 hash;
        int weight;
        int totalWeight;
        int8 level;
        talu32 children;
        int parent;
};

struct input {
        int numPrograms;
        struct program *programs;
        int bottomProgram;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPrograms(int numPrograms, struct program programs[]) {
        for (int i = 0; i < numPrograms; i++) {
                struct program p = programs[i];
                printf("(%d)%s: w=%d, tw=%d, ", i, p.name, p.weight, p.totalWeight);
                printf("l=%d, p=%d, c=( ", p.level, p.parent);
                tal_for(p.children, i) {
                        printf("%u ", p.children.array[i]);
                }
                printf(")\n");
        }
}

u32 nameToHash(const char *name) {
        const u32 seed = 0x12345678;
        return FNV(name, seed);
}

void findParents(int numPrograms, struct program programs[]) {
        for (int i = 0; i < numPrograms; i++) {
                struct program *p = &programs[i];
                if (p->children.length == 0) { continue; }

                tal_for(p->children, j) {
                        u32 child = p->children.array[j];
                        programs[child].parent = i;
                }
        }

}

int setLevels(int numPrograms, struct program programs[]) {
        bool allSet;
        int maxLevel = 0;
        do {
                allSet = true;
                for (int i = 0; i < numPrograms; i++) {
                        struct program *p = &programs[i];
                        if (p->level == -1 && p->children.length == 0) {
                                // Top level is 0
                                allSet = false;
                                p->level = 0;
                                programs[p->parent].level = 1;
                        } else if (p->level != -1) {
                                int parentLevel = programs[p->parent].level;
                                // Set parent level if isnt set or is too low
                                if (parentLevel == -1 || parentLevel == p->level) {
                                        allSet = false;
                                        programs[p->parent].level = p->level + 1;
                                }
                        }
                        if (p->level > maxLevel) {
                                maxLevel = p->level;
                        }
                }
        } while (!allSet);
        return maxLevel;
}

// Returns index of bottom program
int setTotalWeight(int numPrograms, struct program programs[], int maxLevel) {
        int bottomIndex = 0;
        for (int l = 1; l <= maxLevel; l++) {
                for (int i = 0; i < numPrograms; i++) {
                        struct program *p = &programs[i];
                        if (p->level != l) { continue; }
                        tal_for(p->children, j) {
                                u32 child = p->children.array[j];
                                int weight = programs[child].totalWeight;
                                p->totalWeight += weight;
                        }
                        if (l == maxLevel) { bottomIndex = i; }
                }
        }
        return bottomIndex;
}

int findCorrectWeight(int numPrograms, struct program programs[], int index, int correctWeight) {
        // Can be a max of 2 different weights. If a majority of childen is 1 weight, 
        // that is the correct weight, else use the correct weight from parameters
        struct program p = programs[index];

        int weight1 = -1;
        int weight2 = -1;
        int numPrograms1 = 0;
        int numPrograms2 = 0;
        int index1 = -1;
        int index2 = -1;

        tal_for(p.children, i) {
                int child = p.children.array[i];
                int weight = programs[child].totalWeight;
                if (weight1 == -1) {
                        weight1 = weight;
                        numPrograms1++;
                        index1 = child;
                } else if (weight1 == weight) {
                        numPrograms1++;
                } else if (weight2 == -1) {
                        weight2 = weight;
                        numPrograms2++;
                        index2 = child;
                } else {
                        numPrograms2++;
                }
        }

        // If only 1 weight found, this program has the incorrect weight
        // return the correct weight for this program (not including children weight)
        if (numPrograms2 == 0) {
                int weightDiff = correctWeight - p.totalWeight;
                return p.weight + weightDiff;
        }

        if (numPrograms1 != numPrograms2) {
                if (numPrograms1 == 1) {
                        return findCorrectWeight(numPrograms, programs, index1, weight2);
                } else {
                        return findCorrectWeight(numPrograms, programs, index2, weight1);
                }
        } else {
                // Only 2 children
                int correctChildWeight = (correctWeight - p.weight) / 2;
                if (weight1 != correctChildWeight) {
                        return findCorrectWeight(numPrograms, programs, index1, correctChildWeight);
                } else {
                        return findCorrectWeight(numPrograms, programs, index2, correctChildWeight);
                }
        }
}

void part1(struct input *input) {
        int numPrograms = input->numPrograms;
        struct program *programs = input->programs;
        // printPrograms(numPrograms, programs);

        findParents(numPrograms, programs);
        // printPrograms(numPrograms, programs);

        struct program bottom; 
        for (int i = 0; i < numPrograms; i++) {
                if (programs[i].parent == -1) {
                        bottom = programs[i];
                        input->bottomProgram = i;
                        break;
                }
        }

        printf("Part 1: Bottom program = %s\n\n", bottom.name);
}

void part2(struct input *input) {
        int numPrograms = input->numPrograms;
        struct program *programs = input->programs;
        // printPrograms(numPrograms, programs);

        // Give each program a level, 0 at the top and increasing as we go down
        int maxLevel = setLevels(numPrograms, programs);

        // Go down the levels adding weights of children to total
        setTotalWeight(numPrograms, programs, maxLevel);
        // printPrograms(numPrograms, programs);

        int correctWeight = findCorrectWeight(numPrograms, programs, input->bottomProgram, 0);

        printf("Part 2: Correct Weight = %d\n", correctWeight);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numPrograms = ll->length;
        input.programs = calloc(input.numPrograms, sizeof(struct program));

        u32 hashToIndex[input.numPrograms];

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                struct program *program = &input.programs[i];
                program->level = -1;
                program->parent = -1;

                char *tok = strtok(str, " ");
                program->name = malloc(strlen(tok + 1));
                strcpy(program->name, tok);
                program->hash = nameToHash(tok);
                hashToIndex[i] = program->hash;

                tok = strtok(NULL, ")") + 1;
                program->weight = strtol(tok, NULL, 10);
                program->totalWeight = program->weight;

                tok = strtok(NULL, " ");
                bool arrow = true;
                while (tok != NULL) {
                        if (arrow) {
                                arrow = false;
                        } else {
                                tal_add(program->children, nameToHash(tok));
                        }
                        tok = strtok(NULL, ", ");
                }
                i++;
        }

        // Update children lists from hashes to indexes
        for (int i = 0; i < input.numPrograms; i++) {
                struct program *p = &input.programs[i];
                if (p->children.length == 0) { continue; }

                tal_for(p->children, j) {
                        u32 child = p->children.array[j];
                        for (int k = 0; k < input.numPrograms; k++) {
                                if (hashToIndex[k] == child) {
                                        p->children.array[j] = k;
                                        break;
                                }
                        }
                }
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day07.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

