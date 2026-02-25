/*************************************************
 *File----------Day06.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 25, 2026 19:26:23 UTC
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
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef struct {
        bool answer[26];
} answers;
typedef tll(answers) tllanswer;
typedef tll(tllanswer) tllgroup;

struct input {
        tllgroup groups;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int getAnyAnswered(tllanswer group) {
        answers all = {0};
        tll_foreach(group, it) {
                answers a = it->item;
                for (int i = 0; i < 26; i++) {
                        if (a.answer[i])
                                all.answer[i] = true;
                }
        }

        int numYes = 0;
        for (int i = 0; i < 26; i++) {
                if (all.answer[i])
                        numYes++;
        }
        return numYes;
}

int getAllAnswered(tllanswer group) {
        answers all = tll_front(group);
        tll_foreach(group, it) {
                answers a = it->item;
                for (int i = 0; i < 26; i++) {
                        if (!a.answer[i])
                                all.answer[i] = false;
                }
        }

        int numYes = 0;
        for (int i = 0; i < 26; i++) {
                if (all.answer[i])
                        numYes++;
        }
        return numYes;
}

void part1(struct input input) {
        tllgroup groups = input.groups;

        int answerSum = 0;
        tll_foreach(groups, it) {
                tllanswer group = it->item;
                answerSum += getAnyAnswered(group);
        }

        printf("Part 1: %d\n\n", answerSum);
}

void part2(struct input input) {
        tllgroup groups = input.groups;

        int answerSum = 0;
        tll_foreach(groups, it) {
                tllanswer group = it->item;
                answerSum += getAllAnswered(group);
        }

        printf("Part 2: %d\n\n", answerSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        tllgroup groups = tll_init();

        tllanswer curGroup = tll_init();
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                if (strlen(str) == 0) {
                        tll_push_back(groups, curGroup);
                        curGroup = (tllanswer)tll_init();
                        continue;
                }

                answers ans = {0};
                for (int i = 0; i < (int)strlen(str); i++) {
                        int ansIndex = str[i] - 'a';
                        ans.answer[ansIndex] = true;
                }
                tll_push_back(curGroup, ans);
        }
        tll_push_back(groups, curGroup);

        input.groups = groups;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day06.txt";
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

