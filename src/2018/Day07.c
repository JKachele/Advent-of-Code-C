/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 17, 2026 12:22:01 EDT
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
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(ivec2) talivec2;

struct step {
        bool valid;
        bool working;
        bool complete;
        bool prereqs[26];
        int numPrereqs;
};

struct worker {
        bool working;
        int step;
        int timeLeft;
};

struct input {
        talivec2 orders;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printSteps(struct step steps[26]) {
        for (int i = 0; i < 26; i++) {
                struct step step = steps[i];
                if (!step.valid) continue;
                printf("Step %c: [", i + 'A');
                for (int j = 0; j < 26; j++) {
                        if (step.prereqs[j])
                                printf("%c", j + 'A');
                }
                printf("] %d\n", step.numPrereqs);
        }
}

void printWorkers(const int NumWorkers, struct worker workers[], int second, char *done) {
        if (!Debug) return;
        printf(" %04d  ", second);
        for (int i = 0; i < NumWorkers; i++) {
                char step = workers[i].working ? workers[i].step + 'A' : '.';
                int time = workers[i].working ? workers[i].timeLeft : 0;
                printf("   %c %02d  ", step, time);
        }
        printf("%s\n", done);
}

void part1(struct input *input) {
        talivec2 orders = input->orders;

        struct step steps[26] = {0};
for (int i = 0; i < (int)orders.length; i++) {
                ivec2 order = orders.array[i];
                // printf("%c -> %c\n", order.x + 'A', order.y + 'A');
                steps[order.x].valid = true;
                steps[order.y].valid = true;
                steps[order.y].prereqs[order.x] = true;
                steps[order.y].numPrereqs++;
        }
        // printSteps(steps);

        // Get step order
        char stepOrder[27] = {0};
        int orderIndex = 0;
        bool done;
        do {
                done = true;
                for (int i = 0; i < 26; i++) {
                        if (!steps[i].valid || steps[i].complete || steps[i].numPrereqs > 0)
                                continue;

                        steps[i].complete = true;
                        for (int j = 0; j < 26; j++) {
                                if (!steps[j].prereqs[i]) continue;
                                steps[j].prereqs[i] = false;
                                steps[j].numPrereqs--;
                        }

                        done = false;
                        stepOrder[orderIndex++] = i + 'A';
                        break;
                }
        } while (!done);

        printf("Part 1: Step Order = %s\n\n", stepOrder);
}

void part2(struct input *input) {
        talivec2 orders = input->orders;
        const int NumWorkers = Debug ? 2 : 5;
        const int BaseTime = Debug ? 0 : 60;

        struct step steps[26] = {0};
        for (int i = 0; i < (int)orders.length; i++) {
                ivec2 order = orders.array[i];
                // printf("%c -> %c\n", order.x + 'A', order.y + 'A');
                steps[order.x].valid = true;
                steps[order.y].valid = true;
                steps[order.y].prereqs[order.x] = true;
                steps[order.y].numPrereqs++;
        }
        // printSteps(steps);

        // Get step order
        char stepOrder[27] = {0};
        int orderIndex = 0;

        struct worker workers[NumWorkers];
        debugp("Second ");
        for (int i = 0; i < NumWorkers; i++) {
                workers[i].working = false;
                debugp("Worker %d ", i);
        }
        debugp("Done\n");

        int numWorkersFree;
        int seconds = -1;
        do {
                printWorkers(NumWorkers, workers, seconds, stepOrder);
                seconds++;

                // If worker is finished, set the step as complete, remove step from prereqs,
                // add step to step order, and set worker as available
                numWorkersFree = 0;
                for (int i = 0; i < NumWorkers; i++) {
                        if (!workers[i].working) {
                                numWorkersFree++;
                                continue;
                        }

                        // Decrement workers time counters
                        workers[i].timeLeft--;

                        if (workers[i].timeLeft == 0) {
                                int step = workers[i].step;
                                stepOrder[orderIndex++] = step + 'A';

                                steps[step].complete = true;
                                for (int j = 0; j < 26; j++) {
                                        if (!steps[j].prereqs[step]) continue;
                                        steps[j].prereqs[step] = false;
                                        steps[j].numPrereqs--;
                                }

                                workers[i].working = false;
                                numWorkersFree++;
                        }
                }
                if (numWorkersFree == 0) continue;

                // If workers available, find first step that can be completed and give to worker
                for (int i = 0; i < 26; i++) {
                        if (!steps[i].valid || steps[i].complete || steps[i].working ||
                                        steps[i].numPrereqs > 0 || numWorkersFree == 0)
                                continue;

                        steps[i].working = true;
                        // Find first free worker
                        for (int j = 0; j < NumWorkers; j++) {
                                if (workers[j].working) continue;
                                workers[j].working = true;
                                workers[j].step = i;
                                workers[j].timeLeft = BaseTime + i + 1;
                                numWorkersFree--;
                                break;
                        }
                }

        } while (numWorkersFree < NumWorkers);

        printf("Part 2: Step Order = %s, Time = %d\n", stepOrder, seconds);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                // Steps are at chars 5 and 36
                ivec2 order;
                order.x = str[5] - 'A';
                order.y = str[36] - 'A';
                tal_add(input.orders, order);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day07.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day07.txt";
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

