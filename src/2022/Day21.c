/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Aug 19, 2025 15:47:19 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

typedef enum {
        ADD,
        SUB,
        MUL,
        DIV,
        EQU
} operation;

typedef struct {
        int64 number;
        int32 monkeys[2];
        operation opp;
} monkey;

typedef struct monkeyNode {
        int64 number;
        operation opp;
        bool dependsOnHumn;
        struct monkeyNode *children[2];
} monkeyNode;

typedef tll(int32) tllint;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool isDigit(char c) {
        return (c >= '0' && c <= '9');
}

int32 strtoid(const char *str) {
        int32 id = (int32)(str[0] - 'a') << 15;
        id += (int32)(str[1] - 'a') << 10;
        id += (int32)(str[2] - 'a') << 5;
        id += (int32)(str[3] - 'a');
        return id;
}

int64 execOpp(int64 num0, int64 num1, operation opp) {
        switch (opp) {
        case ADD:
                return num0 + num1;
        case SUB:
                return num0 - num1;
        case MUL:
                return num0 * num1;
        case DIV:
                return num0 / num1;
        default:
                printf("Invalid operation\n");
                return -1;
        }
}

void simMonkeys(monkey *monkeys, tllint waiting) {
        const int32 root = strtoid("root");
        while (monkeys[root].number == 0) {
                // Loop through waiting monkeys
                tll_foreach(waiting, it) {
                        monkey *m = &monkeys[it->item];
                        if (monkeys[m->monkeys[0]].number == 0 ||
                                        monkeys[m->monkeys[1]].number == 0)
                                continue;
                        int64 nums[2];
                        nums[0] = monkeys[m->monkeys[0]].number;
                        nums[1] = monkeys[m->monkeys[1]].number;
                        m->number = execOpp(nums[0], nums[1], m->opp);
                        tll_remove(waiting, it);
                }
        }
}

monkeyNode *createTree(monkey *monkeys, int32 id) {
        monkeyNode *node = calloc(1, sizeof(monkeyNode));
        monkey m  = monkeys[id];

        // If node is the humn node, mark depends on humn
        if (id == strtoid("humn")) {
                node->number = -1;
                node->dependsOnHumn = true;
                return node;
        }

        // If monkey has a number, it has no children
        if (m.number != 0) {
                node->number = m.number;
                return node;
        }

        // If node is the root node, set it opp to EQU
        if (id == strtoid("root"))
                node->opp = EQU;
        else
                node->opp = m.opp;


        // If number is 0, find 2 children
        node->children[0] = createTree(monkeys, m.monkeys[0]);
        node->children[1] = createTree(monkeys, m.monkeys[1]);

        // if either node depends on humn, this node depends on humn
        if (node->children[0]->dependsOnHumn || node->children[1]->dependsOnHumn) {
                node->dependsOnHumn = true;
        }

        return node;
}

int64 evalBranch(monkeyNode *node) {
        // Can only evaluate if not dependent on humn
        if (node->dependsOnHumn) return -1;

        // Return number if known
        if (node->number != 0) return node->number;

        // Else, exaluate the 2 children
        int64 num0 = evalBranch(node->children[0]);
        int64 num1 = evalBranch(node->children[1]);

        return execOpp(num0, num1, node->opp);
}

int64 calcHumn(monkeyNode *node, int64 evalNum) {
        // Can only evaluate if dependent on humn
        if (!node->dependsOnHumn) return -1;

        // If the humn node, return expected value
        if (node->number == -1)
                return evalNum;

        // Get which nodes requires humn
        int64 num[2];
        num[0] = evalBranch(node->children[0]);
        num[1] = evalBranch(node->children[1]);
        int32 numIndex = num[0] != -1 ? 0 : 1;
        int32 nodeIndex = num[0] != -1 ? 1 : 0;

        int64 childExpected;
        switch (node->opp) {
        case ADD:
                childExpected =  evalNum - num[numIndex];
                break;
        case SUB:
                if (nodeIndex == 0)
                        childExpected =  evalNum + num[numIndex];
                else
                        childExpected =  num[numIndex] - evalNum;
                break;
        case MUL:
                childExpected =  evalNum / num[numIndex];
                break;
        case DIV:
                if (nodeIndex == 0)
                        childExpected =  evalNum * num[numIndex];
                else
                        childExpected =  num[numIndex] / evalNum;
                break;
        case EQU:
                childExpected = num[numIndex];
                break;
        default:
                printf("Invalid operation\n");
                childExpected = -1;
                break;
        }

        int64 humn = calcHumn(node->children[nodeIndex], childExpected);

        return humn;
}

void part1(llist *ll) {
        int32 num = ll->length;
        monkey *monkeys= calloc(0x100000, sizeof(monkey)); // Monkeys stored in a 20 bit id
        tllint waiting = tll_init(); // List of monkeys waiting on others

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                // Get Monkey ID
                char *token = strtok(str, " ");
                int32 id = strtoid(token);
                debugP("%s %d\n", token, id);

                // Get either the Monkey's number or 1st monkey to listen for
                token = strtok(NULL, " ");
                if (isDigit(token[0])) {
                        int64 num = strtol(token, (char**)NULL, 10);
                        monkeys[id].number = num;
                        goto INC;
                }
                // If listening for a monkey, add to list
                tll_push_back(waiting, id);

                int32 monkey1 = strtoid(token);
                monkeys[id].monkeys[0] = monkey1;

                // Get Opperation
                token = strtok(NULL, " ");
                switch (token[0]) {
                case '+':
                        monkeys[id].opp = ADD;
                        break;
                case '-':
                        monkeys[id].opp = SUB;
                        break;
                case '*':
                        monkeys[id].opp = MUL;
                        break;
                case '/':
                        monkeys[id].opp = DIV;
                        break;
                default:
                        printf("UNKNOWN OPPERATOR: %s\n", token);
                }

                // Get 2nd Monkey to listen for
                token = strtok(NULL, "");
                int32 monkey2 = strtoid(token);
                monkeys[id].monkeys[1] = monkey2;
INC:
                current = current->next;
        }

        simMonkeys(monkeys, waiting);
        int64 root = monkeys[strtoid("root")].number;
        free(monkeys);

        printf("Part 1: Root Monkey: %ld\n\n", root);
}

void part2(llist *ll) {
        monkey *monkeys= calloc(0x100000, sizeof(monkey)); // Monkeys stored in a 20 bit id

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                // Get Monkey ID
                char *token = strtok(str, " ");
                int32 id = strtoid(token);
                // debugP("%s %d\n", token, id);

                // Get either the Monkey's number or 1st monkey to listen for
                token = strtok(NULL, " ");
                if (isDigit(token[0])) {
                        int64 num = strtol(token, (char**)NULL, 10);
                        monkeys[id].number = num;
                        goto INC;
                }

                int32 monkey1 = strtoid(token);
                monkeys[id].monkeys[0] = monkey1;

                // Get Opperation
                token = strtok(NULL, " ");
                switch (token[0]) {
                case '+':
                        monkeys[id].opp = ADD;
                        break;
                case '-':
                        monkeys[id].opp = SUB;
                        break;
                case '*':
                        monkeys[id].opp = MUL;
                        break;
                case '/':
                        monkeys[id].opp = DIV;
                        break;
                default:
                        printf("UNKNOWN OPPERATOR: %s\n", token);
                }

                // Get 2nd Monkey to listen for
                token = strtok(NULL, "");
                int32 monkey2 = strtoid(token);
                monkeys[id].monkeys[1] = monkey2;
INC:
                current = current->next;
        }

        monkeyNode *root = createTree(monkeys, strtoid("root"));
        free(monkeys);

        int64 humn = calcHumn(root, 0);

        printf("Part 2: Human Node must be: %ld\n\n", humn);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day21.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day21.txt");
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

