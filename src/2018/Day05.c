/*************************************************
 *File----------Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 16, 2026 09:57:29 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 65536

struct unit {
        int8 type;
        struct unit *next;
        struct unit *prev;
};

struct input {
        struct unit *polymer;
        int len;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPolymer(struct unit *polymer) {
        struct unit *cur = polymer;
        while (cur != NULL) {
                char type = (cur->type < 0) ? (cur->type * -1) + 96 : cur->type + 64;
                printf("%c", type);
                cur = cur->next;
        }
        printf("\n");
}

struct unit *copyPolymer(struct unit *polymer) {
        struct unit *newPolymer = malloc(sizeof(struct unit));
        newPolymer->type = polymer->type;
        newPolymer->next = NULL;
        newPolymer->prev = NULL;

        struct unit *cur = polymer->next;
        struct unit *newCur = newPolymer;
        while (cur != NULL) {
                struct unit *next = malloc(sizeof(struct unit));
                next->type = cur->type;
                next->next = NULL;
                next->prev = newCur;
                newCur->next = next;
                newCur = next;
                cur = cur->next;
        }

        return newPolymer;
}

struct unit *reducePolymer(struct unit *polymer) {
        struct unit *head = polymer;

        int numReductions;
        do {
                struct unit *cur = polymer;
                numReductions = 0;
                while (cur != NULL && cur->next != NULL) {
                        // If same type with opposite polarity, remove both units
                        if (cur->type + cur->next->type == 0) {
                                struct unit *toRemove1 = cur;
                                struct unit *toRemove2 = cur->next;
                                if (cur->prev != NULL)
                                        cur->prev->next = cur->next->next;
                                if (cur->next->next != NULL)
                                        cur->next->next->prev = cur->prev;
                                if (cur == head)
                                        head = cur->next->next;
                                cur = cur->next->next;
                                free(toRemove1);
                                free(toRemove2);
                                numReductions++;
                        } else {
                                cur = cur->next;
                        }
                }
        } while (numReductions > 0);

        return head;
}

int reducePolymer2(struct unit *polymer) {
        struct unit *cur = polymer;
        int numReductions = 0;
        while (cur != NULL && cur->next != NULL) {
                // If same type with opposite polarity, remove both units
                if (cur->type + cur->next->type == 0) {
                        struct unit *toRemove1 = cur;
                        struct unit *toRemove2 = cur->next;
                        if (cur->prev != NULL)
                                cur->prev->next = cur->next->next;
                        if (cur->next->next != NULL)
                                cur->next->next->prev = cur->prev;
                        cur = cur->next->next;
                        free(toRemove1);
                        free(toRemove2);
                        numReductions++;
                } else {
                        cur = cur->next;
                }
        }

        return numReductions;
}

int getPolymerLength(struct unit *polymer) {
        int length = 0;
        struct unit *cur = polymer;
        while (cur != NULL) {
                length++;
                cur = cur->next;
        }
        return length;
}

struct unit *removeType(struct unit *polymer, int type) {
        struct unit *head = polymer;

        struct unit *cur = head;
        while (cur != NULL) {
                if (cur->type == type || cur->type == -type) {
                        struct unit *toRemove = cur;
                        if (cur->prev != NULL)
                                cur->prev->next = cur->next;
                        if (cur->next != NULL)
                                cur->next->prev = cur->prev;
                        if (cur == head)
                                head = cur->next;
                        cur = cur->next;
                        free(toRemove);
                } else {
                        cur = cur->next;
                }
        }

        return head;
}

void freePolymer(struct unit *polymer) {
        struct unit *cur = polymer;
        while (cur != NULL) {
                struct unit *next = cur->next;
                free(cur);
                cur = next;
        }
}

void part1(struct input *input) {
        struct unit *polymer = input->polymer;
        // printPolymer(polymer);

        int numReductions;
        do {
                numReductions = reducePolymer2(polymer);
        } while (numReductions > 0);
        // polymer = reducePolymer(polymer);
        // printPolymer(polymer);

        int numUnits = getPolymerLength(polymer);
        input->len = numUnits;

        printf("Part 1: Polymer Length = %d\n\n", numUnits);
}

void part2(struct input *input) {
        struct unit *polymer = input->polymer;
        // printPolymer(polymer);

        // Get all unit types
        bool unitTypes[27] = {false};
        struct unit *cur = polymer;
        while (cur != NULL) {
                int typeIndex = (cur->type < 0) ? (cur->type * -1) : cur->type;
                unitTypes[typeIndex] = true;
                cur = cur->next;
        }

        // for each unit type, remove all units of that type and reduce the polymer
        int minLength = input->len;
        for (int i = 1; i <= 26; i++) {
                if (!unitTypes[i]) continue;

                struct unit *cur = copyPolymer(polymer);
                cur = removeType(cur, i);
                cur = reducePolymer(cur);
                // printPolymer(cur);

                int length = getPolymerLength(cur);
                if (length < minLength)
                        minLength = length;
                freePolymer(cur);
        }

        printf("Part 2: Shortest Polymer = %d\n", minLength);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.polymer = malloc(sizeof(struct unit));
        struct unit *cur = input.polymer;
        cur->type = (str[0] >= 'a') ? (str[0] - 96) * -1 : str[0] - 64;
        cur->next = NULL;
        cur->prev = NULL;

        for (int i = 1; str[i] != '\0'; i++) {
                struct unit *next = malloc(sizeof(struct unit));
                next->type = (str[i] >= 'a') ? (str[i] - 96) * -1 : str[i] - 64;
                next->next = NULL;
                next->prev = cur;
                cur->next = next;
                cur = next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day05.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day05.txt";
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

