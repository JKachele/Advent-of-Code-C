/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 16, 2025 08:39:17 EDT
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
#include "../util/util.h"

typedef tll(int64) tllint;

typedef enum {
        ADD,
        SUB,
        MULT,
        DIV
} opperation;

typedef struct {
        tllint items;           // Items currently holding
        opperation opp;         // Opperation on the item
        int oppval;             // old [opp] [oppval] (oppval is old if -1)
        int test;               // Test - Divisible by [test]
        int pass;               // Pass to this monkey if test passes
        int fail;               // Pass to this monkey if test fails
        int64 inspections;        // Number if inspections my monkey
} monkey;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

char opp2char(opperation op) {
        switch (op) {
        case ADD:
                return '+';
        case SUB:
                return '-';
        case MULT:
                return '*';
        case DIV:
                return '/';
        default:
                return 0;
        }
}

void printMonkeys(monkey monkeys[], int num) {
        if (!Debug) return;
        for (int i = 0; i < num; i++) {
                monkey m = monkeys[i];
                printf("Monkey %d: ", i);
                printf("[ ");
                tll_foreach(m.items, it) {
                        printf("%ld ", it->item);
                }
                printf("]\n");
                if (m.oppval == -1)
                        printf("\topp: old %c old | ", opp2char(m.opp));
                else
                        printf("\topp: old %c %d | ", opp2char(m.opp), m.oppval);
                printf("Test: div by %d | ", m.test);
                printf("Pass: %d | Fail: %d | ", m.pass, m.fail);
                printf("Inspections: %ld\n", m.inspections);
        }
        printf("\n");
}

void parseItems(char *str, tllint *items) {
        char *itemStr = strtok(str, ",");
        while (itemStr != NULL) {
                // Add 1 to str to remove leading white space
                int64 item = strtol(itemStr + 1, (char**)NULL, 10);
                // debugP("%d ", item);
                tll_push_back(*items, item);
                itemStr = strtok(NULL, ",");
        }
        // debugP("\n");
}

void parseOpp(char *str, opperation *opp, int *oppVal) {
        // opp at char 23
        char oppChar = str[23];
        // debugP("Opp: %c ", oppChar);

        switch (oppChar) {
        case '+':
                *opp = ADD;
                break;
        case '-':
                *opp = SUB;
                break;
        case '*':
                *opp = MULT;
                break;
        case '/':
                *opp = DIV;
                break;
        default:
                printf("INVALID OPP CHAR!\n");
        }

        if (strcmp(str + 25, "old") == 0) {
                *oppVal = -1;
        } else {
                *oppVal = strtol(str + 25, (char**)NULL, 10);
        }
        // debugP("%d\n", *oppVal);
}

void parseMonkeys(monkey monkeys[], int num, llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                
                // Get monkey Number;
                // Offset str by 7 - "Monkey "
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                int monkeyNum = strtol(strtok(str + 7, ":"), (char**)NULL, 10);
                // debugP("Monkey Num %d: ", monkeyNum);
                current = current->next;

                monkey *cur = &monkeys[monkeyNum];

                // Get starting items
                // Offset str by 17 "  Starting items:"
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                parseItems(str + 17, &cur->items);
                current = current->next;

                // Get operation
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                parseOpp(str, &cur->opp, &cur->oppval);
                current = current->next;

                // Get test
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                cur->test = strtol(str + 21, (char**)NULL, 10);
                current = current->next;

                // Get pass
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                cur->pass = strtol(str + 29, (char**)NULL, 10);
                current = current->next;

                // Get fail
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                cur->fail = strtol(str + 30, (char**)NULL, 10);
                current = current->next;

                if (current != NULL)
                        current = current->next;
        }
        printMonkeys(monkeys, num);
}

void initMonkeys(monkey monkeys[], int num) {
        for (int i = 0; i < num; i++) {
                monkey *m = &monkeys[i];
                m->items = (tllint)tll_init();
                m->opp = ADD;
                m->oppval = 0;
                m->test = 0;
                m->pass = 0;
                m->fail = 0;
                m->inspections = 0;
        }
}

int64 getCycle(monkey monkeys[], int num) {
        int64 cycle = 1;

        for (int i = 0; i < num; i++) {
                cycle *= monkeys[i].test;
                debugP("%d\n", cycle);
        }

        return cycle;
}

int64 monkeyOp(int64 item, opperation opp, int oppVal) {
        int64 new = 0;
        if (oppVal == -1)
                oppVal = item;

        switch (opp) {
        case ADD:
                new = item + oppVal;
                break;
        case SUB:
                new = item - oppVal;
                break;
        case MULT:
                new = item * oppVal;
                break;
        case DIV:
                new = item / oppVal;
                break;
        }
        return new;
}

void monkeyRound(monkey monkeys[], int num) {
        for (int i = 0; i < num; i++) {
                monkey *m = &monkeys[i];
                tll_foreach(m->items, it) {
                        int64 item = it->item;
                        item = monkeyOp(item, m->opp, m->oppval);

                        item = item / 3;

                        if ((item % m->test) == 0)
                                tll_push_back(monkeys[m->pass].items, item);
                        else
                                tll_push_back(monkeys[m->fail].items, item);

                        tll_remove(m->items, it);
                        m->inspections++;
                }
        }
}

void monkeyRound2(monkey monkeys[], int num, int64 cycle) {
        for (int i = 0; i < num; i++) {
                monkey *m = &monkeys[i];
                tll_foreach(m->items, it) {
                        int64 item = it->item;
                        item = monkeyOp(item, m->opp, m->oppval);

                        // Keep item value small while not affecting the tests
                        item = item % cycle;

                        if ((item % m->test) == 0)
                                tll_push_back(monkeys[m->pass].items, item);
                        else
                                tll_push_back(monkeys[m->fail].items, item);

                        tll_remove(m->items, it);
                        m->inspections++;
                }
        }
}

void part1(llist *ll) {
        const int NUM_ROUNDS = 20;
        const int NUM_MONKEYS = (ll->length + 1) / 7;
        monkey monkeys[NUM_MONKEYS];

        initMonkeys(monkeys, NUM_MONKEYS);
        parseMonkeys(monkeys, NUM_MONKEYS, ll);

        for (int i = 0; i < NUM_ROUNDS; i++)
                monkeyRound(monkeys, NUM_MONKEYS);
        printMonkeys(monkeys, NUM_MONKEYS);

        int64 highest1 = 0;
        int64 highest2 = 0;
        for (int i = 0; i < NUM_MONKEYS; i++) {
                int64 insp = monkeys[i].inspections;
                if (insp > highest1) {
                        highest2 = highest1;
                        highest1 = insp;
                } else if (insp > highest2) {
                        highest2 = insp;
                }
        }

        int64 monkeyBuisness = highest1 * highest2;

        printf("Part 1: Monkey Buisness: %ld\n\n", monkeyBuisness);
}

void part2(llist *ll) {
        const int NUM_ROUNDS = 10000;
        const int NUM_MONKEYS = (ll->length + 1) / 7;
        monkey monkeys[NUM_MONKEYS];

        initMonkeys(monkeys, NUM_MONKEYS);
        parseMonkeys(monkeys, NUM_MONKEYS, ll);

        int64 cycle = getCycle(monkeys, NUM_MONKEYS);

        for (int i = 0; i < NUM_ROUNDS; i++)
                monkeyRound2(monkeys, NUM_MONKEYS, cycle);
        printMonkeys(monkeys, NUM_MONKEYS);

        int64 highest1 = 0;
        int64 highest2 = 0;
        for (int i = 0; i < NUM_MONKEYS; i++) {
                int64 insp = monkeys[i].inspections;
                if (insp > highest1) {
                        highest2 = highest1;
                        highest1 = insp;
                } else if (insp > highest2) {
                        highest2 = insp;
                }
        }

        int64 monkeyBuisness = highest1 * highest2;

        printf("Part 2: Monkey Buisness: %ld\n\n", monkeyBuisness);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day11.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day11.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
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

