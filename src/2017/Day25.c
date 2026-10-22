/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 22, 2026 12:34:58 EDT
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
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024

struct rule {
        struct {
                u8 write;
                bool moveRight;
                int nextState;
        } rule[2];
};

struct cell {
        u8 value;
        struct cell *right;
        struct cell *left;
};

struct input {
        int startState;
        int numSteps;
        int numRules;
        struct rule *rules;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printRules(int numRules, struct rule rules[]) {
        for (int i = 0; i < numRules; i++) {
                printf("State %d:\n", i);
                for (int j = 0; j < 2; j++) {
                        printf("\tValue %d:\n", j);
                        printf("\t\tWrite %d\n", rules[i].rule[j].write);
                        printf("\t\tMove %s\n", rules[i].rule[j].moveRight ? "Right" : "Left");
                        printf("\t\tNext State = %d\n", rules[i].rule[j].nextState);
                }
                printf("\n");
        }
}

struct cell* newCell(void) {
        struct cell *cell = malloc(sizeof(struct cell));
        cell->value = 0;
        cell->right = NULL;
        cell->left = NULL;
        return cell;
}

void part1(struct input *input) {
        int numSteps = input->numSteps;
        int numRules = input->numRules;
        struct rule *rules = input->rules;
        // printRules(numRules, rules);

        struct cell *cur = newCell();
        int curState = input->startState;
        for (int i = 0; i < numSteps; i++) {
                struct rule r = rules[curState];
                u8 value = cur->value;

                cur->value = r.rule[value].write;

                if (r.rule[value].moveRight) {
                        if (cur->right == NULL) {
                                struct cell *next = newCell();
                                next->left = cur;
                                cur->right = next;
                        }
                        cur = cur->right;
                } else {
                        if (cur->left == NULL) {
                                struct cell *next = newCell();
                                next->right = cur;
                                cur->left = next;
                        }
                        cur = cur->left;
                }

                curState = r.rule[value].nextState;
        }

        // Count all 1s on tape
        // Move head all the way left
        while (cur->left != NULL) {
                cur = cur->left;
        }

        // Move all the way right, counting all 1s
        int checksum = 0;
        while (cur != NULL) {
                if (cur->value == 1) {
                        checksum++;
                }
                cur = cur->right;
        }

        printf("Part 1: Checksum = %d\n\n", checksum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        llNode *cur = ll->head;
        char *str = (char*)cur->data;
        input.startState = str[15] - 'A';
        cur = cur->next;

        str = (char*)cur->data;
        input.numSteps = strtol(strtok(str + 36, " "), NULL, 10);
        cur = cur->next;

        // States are 10 lines long and input has 2 lines at top for initial state and num steps
        input.numRules = (ll->length - 2) / 10;
        input.rules = malloc(input.numRules * sizeof(struct rule));

        for (int i = 0; i < input.numRules; i++) {
                struct rule *rule = &input.rules[i];
                // Get first line of rule for value 0
                cur = cur->next->next;
                for (int j = 0; j < 2; j++) {
                        cur = cur->next;

                        rule->rule[j].write = ((char*)cur->data)[22] - '0';
                        cur = cur->next;

                        rule->rule[j].moveRight = (((char*)cur->data)[27] == 'r');
                        cur = cur->next;

                        rule->rule[j].nextState = ((char*)cur->data)[26] - 'A';
                        cur = cur->next;
                }

        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day25.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(pt1 - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Total: %f\n", 
                        parseTime, pt1Time, totalTime);

        return 0;
}

