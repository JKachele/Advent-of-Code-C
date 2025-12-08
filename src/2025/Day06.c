/*************************************************
 *File----------Day06.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Dec 08, 2025 19:40:13 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/talist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef tal(int) talint;
typedef tal(char*) talstring;

typedef struct prob {
        talint nums;
        bool opp; // false: *, true: +
} prob;
typedef tal(prob) talprob;

struct input {
        talprob probs;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printProbs(talprob probs) {
        if (!Debug) return;
        for (int i = 0; i < (int)probs.length; i++) {
                printf("%c ", probs.array[i].opp ? '+' : '*');
                for (int j = 0; j < (int)probs.array[i].nums.length; j++) {
                        int num = probs.array[i].nums.array[j];
                        printf("%d ", num);
                }
                printf("\n");
        }
}

void part1(struct input input) {
        talprob probs = input.probs;
        printProbs(probs);

        int64 answerSum = 0;
        for (int i = 0; i < (int)probs.length; i++) {
                prob p = probs.array[i];
                if (p.opp) {
                        int64 ans = 0;
                        for (int j = 0; j < (int)p.nums.length; j++) {
                                ans += p.nums.array[j];
                        }
                        answerSum += ans;
                } else {
                        int64 ans = 1;
                        for (int j = 0; j < (int)p.nums.length; j++) {
                                ans *= p.nums.array[j];
                        }
                        answerSum += ans;
                }
        }

        printf("Part 1: %ld\n\n", answerSum);
}

void part2(llist *ll) {
        talstring input = tal_init();
        for (llNode *cur = ll->head; cur->next != NULL; cur = cur->next) {
                char *str = malloc(INPUT_BUFFER_SIZE);
                strncpy(str, (char*)cur->data, INPUT_BUFFER_SIZE);
                tal_add(input, str);
        }
        char opps[INPUT_BUFFER_SIZE];
        strncpy(opps, (char*)ll->tail->data, INPUT_BUFFER_SIZE);

        int numDigits = ll->length - 1;

        talprob probs = tal_init();
        int i = 0;
        prob p = {0};
        while (opps[i] != '\0') {
                int num = 0;
                for (int j = 0; j < numDigits; j++) {
                        if (input.array[j][i] == ' ')
                                continue;
                        num = (num * 10) + (input.array[j][i] - '0');
                }

                if (num != 0) {
                        tal_add(p.nums, num);
                        if (opps[i] != ' ')
                                p.opp = opps[i] == '+';
                }

                if (num == 0 || opps[i+1] == '\0') {
                        tal_add(probs, p);
                        p.nums = (talint)tal_init();
                }

                i++;
        }

        printProbs(probs);

        int64 answerSum = 0;
        for (int i = 0; i < (int)probs.length; i++) {
                prob p = probs.array[i];
                if (p.opp) {
                        int64 ans = 0;
                        for (int j = 0; j < (int)p.nums.length; j++) {
                                ans += p.nums.array[j];
                        }
                        answerSum += ans;
                } else {
                        int64 ans = 1;
                        for (int j = 0; j < (int)p.nums.length; j++) {
                                ans *= p.nums.array[j];
                        }
                        answerSum += ans;
                }
        }

        printf("Part 2: %ld\n\n", answerSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        talprob probs = tal_init();

        char str[INPUT_BUFFER_SIZE];
        strncpy(str, (char*)ll->head->data, INPUT_BUFFER_SIZE);
        char *tok = strtok(str, " ");
        while (tok != NULL) {
                prob p = {0};
                p.nums = (talint)tal_init();
                tal_add(p.nums, strtol(tok, NULL, 10));
                tal_add(probs, p);
                tok = strtok(NULL, " ");
        }

        int line = 1;
        for (llNode *cur = ll->head->next; cur->next != NULL; cur = cur->next) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)cur->data, INPUT_BUFFER_SIZE);

                char *tok = strtok(str, " ");
                int i = 0;
                while (tok != NULL) {
                        int num = strtol(tok, NULL, 10);
                        tal_add(probs.array[i].nums, num);
                        tok = strtok(NULL, " ");
                        i++;
                }
                line++;
        }

        strncpy(str, (char*)ll->tail->data, INPUT_BUFFER_SIZE);
        tok = strtok(str, " ");
        int i = 0;
        while (tok != NULL) {
                probs.array[i].opp = (tok[0] == '+');
                tok = strtok(NULL, " ");
                i++;
        }

        input.probs = probs;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2025/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2025/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
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

