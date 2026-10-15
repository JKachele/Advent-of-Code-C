/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 15, 2026 13:26:12 EDT
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
#include "../util/talist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(int) talint;

struct program {
        talint connecting;
        int group;
};

struct input {
        int numPrograms;
        struct program *programs;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printProgs(int numProgs, struct program progs[]) {
        for (int i = 0; i < numProgs; i++) {
                struct program *p = &progs[i];
                printf("%d: ( ", i);
                tal_for(p->connecting, j) {
                        printf("%d ", p->connecting.array[j]);
                }
                printf(")\n\tGroup %d\n", p->group);
        }
}

void part1(struct input *input) {
        int numPrograms = input->numPrograms;
        struct program *progs = input->programs;
        // printProgs(numPrograms, progs);

        for (int i = 0; i < numPrograms; i++) {
                struct program *p = &progs[i];
                if (p->group == -1) {
                        p->group = i;
                }

                // list of groups to be converted to this group
                talint connectingGroups = tal_init();
                tal_for(p->connecting, j) {
                        struct program *g = &progs[p->connecting.array[j]];
                        if (g->group == -1) {
                                g->group = p->group;
                        } else {
                                tal_add(connectingGroups, g->group);
                        }
                }

                tal_for(connectingGroups, j) {
                        for (int k = 0; k < numPrograms; k++) {
                                struct program *g = &progs[k];
                                if (g->group == connectingGroups.array[j]) {
                                        g->group = p->group;
                                }
                        }
                }
        }
        // printProgs(numPrograms, progs);

        // Count programs in same group as program 0
        int group = progs[0].group;
        int groupCount = 0;
        for (int i = 0; i < numPrograms; i++) {
                if (progs[i].group == group)
                        groupCount++;
        }

        printf("Part 1: Programs in group with 0 = %d\n\n", groupCount);
}

void part2(struct input *input) {
        int numPrograms = input->numPrograms;
        struct program *progs = input->programs;
        // printProgs(numPrograms, progs);

        // Count number of groups
        bool isGroup[numPrograms];

        for (int i = 0; i < numPrograms; i++) {
                if (progs[i].group == i) {
                        isGroup[i] = true;
                } else {
                        isGroup[i] = false;
                }
        }

        int numGroups = 0;
        for (int i = 0; i < numPrograms; i++) {
                if (isGroup[i])
                        numGroups++;
        }

        printf("Part 2: Groups = %d\n", numGroups);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numPrograms = ll->length;
        input.programs = calloc(input.numPrograms, sizeof(struct program));
        struct program *progs = input.programs;

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                strtok(str, " ");
                strtok(NULL, " ");
                char *tok = strtok(NULL, ", ");
                while (tok != NULL) {
                        tal_add(progs[i].connecting, strtol(tok, NULL, 10));
                        tok = strtok(NULL, ", ");
                }
                progs[i].group = -1;

                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day12.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day12.txt";
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

