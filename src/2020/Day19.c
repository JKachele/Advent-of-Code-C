/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 18, 2026 17:51:32 UTC
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
#include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096
#define NUM_RULES 256

typedef tll(int) tllint;
typedef tll(char*) tllstr;

typedef struct rule {
        bool isChar;
        char c;
        tllint rules[2];
} rule;

struct input {
        rule rules[NUM_RULES];
        tllstr msgs;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printRules(rule rules[], int numRules) {
        for (int i = 0; i < numRules; i++) {
                rule r = rules[i];
                printf("%d: ", i);
                if (r.isChar) {
                        printf("\"%c\"", r.c);
                } else {
                        tll_foreach(r.rules[0], it)
                                printf("%d ", it->item);
                        if (tll_length(r.rules[1]) != 0) {
                                printf("| ");
                                tll_foreach(r.rules[1], it)
                                        printf("%d ", it->item);
                        }
                }
                printf("\n");
        }
        printf("\n");
}

void printMsgs(tllstr msgs) {
        tll_foreach(msgs, it) {
                printf("%s\n", it->item);
        }
        printf("\n");
}

bool msgValid(rule rules[], int ruleIndex, char *msg, int *msgIndex) {
        rule curRule = rules[ruleIndex];

        if (*msgIndex >= (int)strlen(msg))
                return false;
        if (curRule.isChar)
                return msg[*msgIndex] == curRule.c;

        for (int i = 0; i < 2; i++) {
                tllint rList = curRule.rules[i];
                if (tll_length(rList) == 0)
                        break;

                bool listValid = true;
                int newMsgIndex = *msgIndex;
                tll_foreach(rList, it) {
                        int ruleNum = it->item;
                        if (!msgValid(rules, ruleNum, msg, &newMsgIndex)) {
                                listValid = false;
                                break;
                        }
                        newMsgIndex++;
                }
                if (listValid) {
                        *msgIndex = newMsgIndex - 1;
                        return true;
                }
        }

        return false;
}

void part1(struct input input) {
        // printRules(input.rules, 6);
        // printMsgs(input.msgs);

        int numValid = 0;
        tll_foreach(input.msgs, it) {
                char *msg = it->item;
                int index = 0;
                bool valid = msgValid(input.rules, 0, msg, &index);
                if ((int)strlen(msg) - 1 != index)
                        valid = false;
                debugp("%s: %s\n", msg, valid ? "Valid" : "Invalid");
                if (valid) numValid++;
        }

        printf("Part 1: %d\n\n", numValid);
}

void part2(struct input input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        rule *rules = input.rules;

        // Parse rules
        llNode *cur = ll->head;
        for (; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (strlen(str) == 0) break;

                int ruleId = strtol(strtok(str, ":"), NULL, 10);
                rule *r = &rules[ruleId];
                char *params = strtok(NULL, "") + 1; // +1 to remove space
                                                     //
                if (params[0] == '"') {
                        r->isChar = true;
                        r->c = params[1];
                        continue;
                }

                r->isChar = false;
                int i = 0;
                char *ruleNumStr = strtok(params, " ");
                while (ruleNumStr != NULL) {
                        if (ruleNumStr[0] == '|') {
                                i = 1;
                        } else {
                                int ruleNum = strtol(ruleNumStr, NULL, 10);
                                tll_push_back(r->rules[i], ruleNum);
                        }
                        ruleNumStr = strtok(NULL, " ");
                }
        }
        cur = cur->next;

        // Parse messages
        for (; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                int len = strlen(str);
                char *msg = malloc((len + 1) * sizeof(char));
                strncpy(msg, str, len);
                msg[len] = '\0';
                tll_push_back(input.msgs, msg);
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day19.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day19.txt";
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

