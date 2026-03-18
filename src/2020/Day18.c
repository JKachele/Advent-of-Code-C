/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 17, 2026 21:17:39 UTC
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

typedef struct eqfield {
        bool isNum; // number : expression
        union {
                u64 num;
                struct eqfield *exp;
        };
        bool add; // add : mul
        struct eqfield *prev;
        struct eqfield *next;
} eqfield;

struct input {
        int len;
        char **strs1;
        char **strs2;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printEq(eqfield *root, bool top) {
        if (!Debug) return;
        eqfield *cur = root;
        while (cur != NULL) {
                if (cur->prev == NULL) {
                        if (cur->isNum) {
                                printf("%lu", cur->num);
                        } else {
                                printf("(");
                                printEq(cur->exp, false);
                                printf(")");
                        }
                } else {
                        printf(" %c ", cur->prev->add? '+' : '*');
                        if (cur->isNum) {
                                printf("%lu", cur->num);
                        } else {
                                printf("(");
                                printEq(cur->exp, false);
                                printf(")");
                        }
                }
                cur = cur->next;
        }
        if (top) printf("\n");
}

void freeEq(eqfield *root) {
        if (root == NULL) return;

        if (!root->isNum)
                freeEq(root->exp);
        freeEq(root->next);
        free(root);
}

bool isNumber(char c) {
        return c >= '0' && c <= '9';
}

void parseEq(char **str, eqfield *root) {
        eqfield *cur = root;
        while (**str != '\0') {
                if (isNumber(**str)) {
                        cur->isNum = true;
                        cur->num = **str - '0';
                } else if (**str == '+') {
                        cur->add = true;
                        cur->next = calloc(1, sizeof(eqfield));
                        cur->next->prev = cur;
                        cur = cur->next;
                } else if (**str == '*') {
                        cur->add = false;
                        cur->next = calloc(1, sizeof(eqfield));
                        cur->next->prev = cur;
                        cur = cur->next;
                } else if (**str == '(') {
                        cur->isNum = false;
                        cur->exp = calloc(1, sizeof(eqfield));
                        (*str)++;
                        parseEq(str, cur->exp);
                } else if (**str == ')') {
                        return;
                }
                (*str)++;
        }
}

u64 eval(eqfield **root) {
        // Evaluate all inner expressions first
        eqfield *cur = *root;
        while (cur != NULL) {
                if (!cur->isNum) {
                        u64 num = eval(&(cur->exp));
                        cur->isNum = true;
                        freeEq(cur->exp);
                        cur->num = num;
                }
                cur = cur->next;
        }
        cur = *root;

        u64 ans = 0;
        u64 lhs = 0;
        u64 rhs = 0;
        while (cur != NULL) {
                if (cur->prev == NULL) {
                        ans = cur->num;
                } else {
                        lhs = ans;
                        rhs = cur->num;
                        ans = cur->prev->add ? (lhs + rhs) : (lhs * rhs);
                        // printf("%d %c %d = %d\n", lhs, cur->prev->add? '+' : '*', rhs, ans);
                }
                cur = cur->next;
        }
        return ans;
}

u64 advEval(eqfield **root) {
        eqfield *cur = *root;
        while (cur != NULL) {
                if (!cur->isNum) {
                        u64 num = advEval(&(cur->exp));
                        cur->isNum = true;
                        freeEq(cur->exp);
                        cur->num = num;
                }
                cur = cur->next;
        }
        cur = *root;

        // Compute all additions first
        while (cur != NULL) {
                if (cur->next != NULL && cur->add) {
                        eqfield *next = cur->next;
                        cur->num += next->num;
                        cur->add = next->add;
                        cur->next = next->next;
                        if (cur->next != NULL)
                                cur->next->prev = cur;
                        free(next);
                } else {
                        cur = cur->next;
                }
        }
        cur = *root;

        // Finish evaluation with multiplications
        u64 ans = 0;
        while (cur != NULL) {
                if (cur->prev == NULL) {
                        ans = cur->num;
                } else {
                        ans *= cur->num;
                }
                cur = cur->next;
        }
        return ans;
}

void part1(struct input input) {
        char **in = input.strs1;

        u64 ansSum = 0;
        for (int i = 0; i < input.len; i++) {
                eqfield *root = calloc(1, sizeof(eqfield));

                parseEq(&in[i], root);
                printEq(root, false);
                u64 ans = eval(&root);
                ansSum += ans;
                debugp(" = %lu\n", ans);

                freeEq(root);
        }

        printf("Part 1: %lu\n\n", ansSum);
}

void part2(struct input input) {
        char **in = input.strs2;

        u64 ansSum = 0;
        for (int i = 0; i < input.len; i++) {
                eqfield *root = calloc(1, sizeof(eqfield));

                parseEq(&in[i], root);
                printEq(root, false);
                u64 ans = advEval(&root);
                ansSum += ans;
                debugp(" = %lu\n", ans);

                freeEq(root);
        }

        printf("Part 2: %lu\n", ansSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.len = ll->length;
        input.strs1 = calloc(ll->length, sizeof(char*));
        input.strs2 = calloc(ll->length, sizeof(char*));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                int len = strlen(str);
                input.strs1[i] = malloc((len + 1) * sizeof(char));
                input.strs2[i] = malloc((len + 1) * sizeof(char));
                strncpy(input.strs1[i], str, len);
                strncpy(input.strs2[i], str, len);
                input.strs1[i][len] = '\0';
                input.strs2[i][len] = '\0';
                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day18.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day18.txt";
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

