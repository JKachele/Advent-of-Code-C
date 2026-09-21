/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 21, 2026 15:00:17 EDT
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

typedef tal(bool) talbool;

struct pot {
        int num;
        bool plant;
};
typedef tal(struct pot) talpot;

struct input {
        bool rules[32];
        talpot pots;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPots(talpot pots) {
        tal_for(pots, i) {
                printf("%c", pots.array[i].plant ? '#' : '.');
        }
        printf("\n");
}

talpot copyPots(talpot pots) {
        talpot copy = tal_init();
        tal_for(pots, i) {
                tal_add(copy, pots.array[i]);
        }
        return copy;
}

bool isPot(talpot pots, bool rules[32], int index) {
        int ruleIndex = 0;
        for (int i = index - 2; i <= index + 2; i++) {
                ruleIndex *= 2;
                if (i < 0 || i >= (int)pots.length)
                        continue;
                ruleIndex += pots.array[i].plant ? 1 : 0;
        }
        return rules[ruleIndex];
}

talpot stepPots(talpot pots, bool rules[32]) {
        talpot newPots = tal_init();

        // Check 2 pots to the left if there will be a plant there;
        bool pot2 = isPot(pots, rules, -2);
        bool pot1 = isPot(pots, rules, -1);
        if (pot2) {
                struct pot new2 = {pots.array[0].num - 2, true};
                struct pot new1 = {pots.array[0].num - 1, false};
                tal_add(newPots, new2);
                if (pot1)
                        new1.plant = true;
                tal_add(newPots, new1);
        } else if (pot1) {
                struct pot new1 = {pots.array[0].num - 1, true};
                tal_add(newPots, new1);
        }

        // Check pots currently in array
        int lastNum = 0;
        for (int i = 0; i < (int)pots.length; i++) {
                struct pot new = {pots.array[i].num, isPot(pots, rules, i)};
                tal_add(newPots, new);
                lastNum = pots.array[i].num;
        }

        // Check 2 pots to the right if there will be a plant there;
        pot2 = isPot(pots, rules, pots.length+1);
        pot1 = isPot(pots, rules, pots.length);

        if (pot2) {
                struct pot new2 = {lastNum + 2, true};
                struct pot new1 = {lastNum + 1, false};
                if (pot1)
                        new1.plant = true;
                tal_add(newPots, new1);
                tal_add(newPots, new2);
        } else if (pot1) {
                struct pot new1 = {lastNum + 1, true};
                tal_add(newPots, new1);
        }

        tal_destroy(pots);

        return newPots;
}

int getPlantSum(talpot pots) {
        int plantSum = 0;
        tal_for(pots, i) {
                if (pots.array[i].plant)
                        plantSum += pots.array[i].num;
        }
        return plantSum;
}

void part1(struct input *input) {
        const int NumSteps = 20;
        talpot pots = copyPots(input->pots);
        bool *rules = input->rules;
        // printPots(pots);

        for (int i = 0; i < NumSteps; i++) {
                pots = stepPots(pots, rules);
                // printPots(pots);
        }

        int plantSum = getPlantSum(pots);

        printf("Part 1: Plant Sum = %d\n\n", plantSum);
}

void part2(struct input *input) {
        const long NumSteps = 50000000000;
        talpot pots = input->pots;
        bool *rules = input->rules;

        // Check first 150 steps for a pettern to the plant sum
        // Once diff stabablizes, exit and use to calculate to 50,000,000,000
        int lastPlantSum = getPlantSum(pots);
        int lastDiff[3] = {0};
        int step;
        for (step = 1; step < 150; step++) {
                pots = stepPots(pots, rules);
                int plantSum = getPlantSum(pots);
                int diff = plantSum - lastPlantSum;
                // printf("Sum: %d, Diff %d\n", plantSum, diff);
                lastPlantSum = plantSum;
                lastDiff[2] = lastDiff[1];
                lastDiff[1] = lastDiff[0];
                lastDiff[0] = diff;
                if (lastDiff[2] == lastDiff[1] && lastDiff[1] == lastDiff[0])
                        break;
        }
        // printf("Step %d, Sum %d, diff %d\n", step, lastPlantSum, lastDiff[0]);

        long stepsLeft = NumSteps - step;
        long plantSum = lastPlantSum + (lastDiff[0] * stepsLeft);

        printf("Part 2: Plant Sum = %ld\n", plantSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        // Get initial state
        char *str = (char*)ll->head->data;
        str = str + 15;

        for (int i = 0; i < (int)strlen(str); i++) {
                struct pot pot = {i, (str[i] == '#')};
                tal_add(input.pots, pot);
        }

        for (int i = 0; i < 32; i++)
                input.rules[i] = false;

        for (llNode *cur = ll->head->next->next; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                int ruleIndex = 0;
                for (int i = 0; i < 5; i++) {
                        ruleIndex *= 2;
                        if (str[i] == '#')
                                ruleIndex++;
                }

                input.rules[ruleIndex] = (str[9] == '#');
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day12.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day12.txt";
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

