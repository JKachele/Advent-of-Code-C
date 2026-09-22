/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Sep 22, 2026 08:24:38 EDT
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

typedef tal(u8) talu8;

struct input {
        talu8 recipes;
        int recipeNum;
        int e1;
        int e2;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printRecipes(talu8 *recipes, int e1, int e2) {
        tal_for(*recipes, i) {
                if ((int)i == e1)
                        printf("(%d)", recipes->array[i]);
                else if ((int)i == e2)
                        printf("[%d]", recipes->array[i]);
                else
                        printf(" %d ", recipes->array[i]);
        }
        printf("\n");
}

int countDigits(int i) {
        if (i == 0) return 1;

        int count = 0;
        while (i != 0) {
                i /= 10;
                count++;
        }
        return count;
}

void makeRecipes(talu8 *recipes, int *e1, int *e2) {
        u8 cur1 = recipes->array[*e1];
        u8 cur2 = recipes->array[*e2];
        int recipeSum = cur1 + cur2;
        if (recipeSum > 9)
                tal_add(*recipes, recipeSum / 10);
        tal_add(*recipes, recipeSum % 10);
        *e1 = (*e1 + cur1 + 1) % recipes->length;
        *e2 = (*e2 + cur2 + 1) % recipes->length;
}

void part1(struct input *input) {
        talu8 *recipes = &input->recipes;
        int recipeNum = input->recipeNum;
        int *e1 = &input->e1;
        int *e2 = &input->e2;
        // printRecipes(recipes, *e1, *e2);

        while ((int)recipes->length < recipeNum + 10) {
                makeRecipes(recipes, e1, e2);
                // printRecipes(recipes, *e1, *e2);
        }

        long finalScore = 0;
        for (int i = recipeNum; i < recipeNum + 10; i++) {
                finalScore *= 10;
                finalScore += recipes->array[i];
        }

        printf("Part 1: Final Score = %ld\n\n", finalScore);
}

void part2(struct input *input) {
        talu8 *recipes = &input->recipes;
        int *e1 = &input->e1;
        int *e2 = &input->e2;
        // printRecipes(recipes, *e1, *e2);

        int numDigits = countDigits(input->recipeNum);
        int recipeNum[numDigits];
        for (int i = numDigits - 1; i >= 0; i--) {
                recipeNum[i] = input->recipeNum % 10;
                input->recipeNum /= 10;
        }

        int i = 0;
        for (;;) {
                bool found = true;
                for (int j = 0; j < numDigits; j++) {
                        if (recipes->array[i+j] != recipeNum[j]) {
                                found = false;
                                break;
                        }
                }
                if (found)
                        break;

                i++;
                if (i >= (int)recipes->length - numDigits)
                        makeRecipes(recipes, e1, e2);
        }

        printf("Part 2: Num Recipes = %d\n", i);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.recipeNum = strtol(str, NULL, 10);
        tal_add(input.recipes, 3);
        tal_add(input.recipes, 7);
        input.e1 = 0;
        input.e2 = 1;


        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day14.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day14.txt";
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

