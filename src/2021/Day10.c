/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 18, 2025 15:30:45 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/quicksort.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(char*) tllstr;
typedef tll(char) tllchar;
typedef tll(int64) tllint64;

struct input {
        tllstr lines;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int32 getCorruptedScore(char *line) {
        tllchar stack = tll_init();
        const char *Open = "([{<";
        for (int i=0; i<(int)strlen(line); i++) {
                if (strchr(Open, line[i]) != NULL) {
                        tll_push_front(stack, line[i]);
                        continue;
                }
                // If closing symbol
                switch (line[i]) {
                case ')':
                        if (tll_front(stack) == '(')
                                tll_pop_front(stack);
                        else
                                return 3;
                        break;
                case ']':
                        if (tll_front(stack) == '[')
                                tll_pop_front(stack);
                        else
                                return 57;
                        break;
                case '}':
                        if (tll_front(stack) == '{')
                                tll_pop_front(stack);
                        else
                                return 1197;
                        break;
                case '>':
                        if (tll_front(stack) == '<')
                                tll_pop_front(stack);
                        else
                                return 25137;
                        break;
                default:
                        printf("Invalid character: %c\n", line[i]);
                }
        }
        // If no corruptions, return 0
        return 0;
}

int64 getIncompleteScore(char *line) {
        tllchar stack = tll_init();
        const char *Open = "([{<";
        for (int i=0; i<(int)strlen(line); i++) {
                if (strchr(Open, line[i]) != NULL) {
                        tll_push_front(stack, line[i]);
                        continue;
                }
                // If closing symbol
                switch (line[i]) {
                case ')':
                        if (tll_front(stack) == '(')
                                tll_pop_front(stack);
                        else
                                return 0;
                        break;
                case ']':
                        if (tll_front(stack) == '[')
                                tll_pop_front(stack);
                        else
                                return 0;
                        break;
                case '}':
                        if (tll_front(stack) == '{')
                                tll_pop_front(stack);
                        else
                                return 0;
                        break;
                case '>':
                        if (tll_front(stack) == '<')
                                tll_pop_front(stack);
                        else
                                return 0;
                        break;
                default:
                        printf("Invalid character: %c\n", line[i]);
                }
        }

        int64 score = 0;
        tll_foreach(stack, it) {
                score *= 5;
                switch(it->item) {
                case '(':
                        score += 1;
                        break;
                case '[':
                        score += 2;
                        break;
                case '{':
                        score += 3;
                        break;
                case '<':
                        score += 4;
                        break;
                }
        }
        return score;
}

void part1(struct input input) {
        tllstr lines = input.lines;

        int32 totalScore = 0;
        tll_foreach(lines, it) {
                int32 score = getCorruptedScore(it->item);
                // printf("%d: %s\n", score, it->item);
                totalScore += score;
        }

        printf("Part 1: %d\n\n", totalScore);
}

void part2(struct input input) {
        tllstr lines = input.lines;

        tllint64 scores = tll_init();
        tll_foreach(lines, it) {
                int64 score = getIncompleteScore(it->item);
                // printf("%ld: %s\n", score, it->item);
                if (score > 0)
                        tll_push_back(scores, score);
        }

        int64 scoreArr[tll_length(scores)];
        int i = 0;
        tll_foreach(scores, it) {
                scoreArr[i] = it->item;
                i++;
        }
        lquicksort(scoreArr, 0, tll_length(scores)-1);

        int64 middleScore = scoreArr[tll_length(scores)/2];
        printf("Part 2: %ld\n\n", middleScore);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;
                tll_push_back(input.lines, str);
                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day10.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();
        part2(input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

