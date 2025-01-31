/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Jan 28, 2025 17:14:07 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
// #include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define A 0
#define B 1
#define C 2

typedef tll(int) tllint;

void printTllint(tllint i) {
        tll_foreach(i, it) {
                printf("%d ", it->item);
        }
        printf("\n");
}

int getTllIndex(tllint ll, int index) {
        int i = 0;
        int out = 0;
        tll_foreach(ll, id) {
                if (i == index)
                        out = id->item;
                i++;
        }
        return out;
}

long combo(long reg[3], int operand) {
        long out;
        if (operand < 4)
                out = operand;
        else if (operand < 7)
                out = reg[operand - 4];
        else
                out = -1;
        return out;
}

void xdv(long reg[3], int operand, int outReg) {
        long denominator = combo(reg, operand);
        denominator = ipow(2, denominator);

        long answer = reg[0] / denominator;
        reg[outReg] = answer;
}

tllint runProgram(long reg[3], tllint program) {
        // Convert program to int array
        int programLength = tll_length(program);
        int p[programLength];
        int instPointer = 0;
        tll_foreach(program, inst) {
                p[instPointer] = inst->item;
                instPointer++;
        }
        instPointer = 0;
        tllint output = tll_init();
        while (instPointer < programLength - 1) {
                long opcode = p[instPointer];
                long operand = p[instPointer + 1];
                int jmpFlag = 0; // 1 if jump happened

                switch (opcode) {
                case 0: // adv
                        xdv(reg, operand, A);
                        break;
                case 1: // bxl
                        reg[B] = reg[B] ^ operand;
                        break;
                case 2: // bst
                        reg[B] = combo(reg, operand) % 8;
                        break;
                case 3: // jnz
                        if (reg[A] != 0) {
                                instPointer = operand;
                                jmpFlag = 1;
                        }
                        break;
                case 4: // bxc
                        reg[B] = reg[B] ^ reg[C];
                        break;
                case 5: // out
                        tll_push_back(output, combo(reg, operand) % 8);
                        break;
                case 6: // bdv
                        xdv(reg, operand, B);
                        break;
                case 7: // cdv
                        xdv(reg, operand, C);
                        break;
                default:
                        break;
                }

                if (jmpFlag == 0)
                        instPointer += 2;
        }

        return output;
}

long findNextDigit(tllint program, long start, int index, int occur) {
        printf("****************************************\n");
        printf("Index: %d\n", index);
        tllint output;
        long next = start;
        long out = 0;
        int occurCount = 0;
        for (;next <= start + ipow(8, index + 1); next += ipow(8, index)) {
                long reg[] = {next, 0, 0};
                printf("%ld: ", reg[A]);
                output = runProgram(reg, program);
                printTllint(output);
                int outD = getTllIndex(output, index);
                int progD = getTllIndex(program, index);
                if (outD == progD) {// && out == start) {
                        printf("********\n");
                        if (occurCount == occur) {
                                out = next;
                                printTllint(output);
                                printTllint(program);
                                break;
                        }
                        occurCount++;
                }
        }
        return out;
}

int findStartVal(tllint program, long startA, long startI) {
        if (startI < 0)
                return startA;

        long startVal = 0;
        for (int i = 0; i < 8; i++) {
                long a = findNextDigit(program, startA, startI, i);
                if (a != 0)
                        startVal += findStartVal(program, a, startI - 1);
                else
                        break;
        }
        return 0;
}

void part1(llist *ll) {
        long reg[3] = {0};
        tllint program = tll_init();

        llNode *current = ll->head;
        char str[BUFFER_SIZE];

        for (int i = 0; i < 3; i++) {
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                strtok(str, ":");
                reg[i] = strtol(strtok(NULL, "") + 1, (char**)NULL, 10);
                current = current->next;
        }
        current = current->next;
        // printf("%d, %d, %d\n", reg[0], reg[1], reg[2]);

        strncpy(str, (char*)current->data, BUFFER_SIZE);
        strtok(str, ":");
        char *tok = strtok(NULL, ",") + 1;
        while (tok != NULL) {
                int inst = strtol(tok, (char**)NULL, 10);
                tll_push_back(program, inst);
                tok = strtok(NULL, ",");
        }
        // tll_foreach(program, inst) {printf("%d ", inst->item);}printf("\n");

        tllint output = runProgram(reg, program);
        char outStr[tll_length(output)];
        int count = 0;
        tll_foreach(output, num) {
                outStr[count] = '0' + num->item;
                outStr[count + 1] = ',';
                count += 2;
        }
        outStr[tll_length(output) * 2 - 1] = '\0';

        printf("Part 1: Output: %s\n\n", outStr);
}

void part2(llist *ll) {
        long reg[3] = {0};
        tllint program = tll_init();

        char str[BUFFER_SIZE];
        strncpy(str, (char*)ll->tail->data, BUFFER_SIZE);
        strtok(str, ":");
        char *tok = strtok(NULL, ",") + 1;
        while (tok != NULL) {
                int inst = strtol(tok, (char**)NULL, 10);
                tll_push_back(program, inst);
                tok = strtok(NULL, ",");
        }
        int progLen = tll_length(program);
        printTllint(program);

        tllint output;
        long a;
        long a2;
        for (a = 1; a > 0; a *= 2) {
                reg[A] = a;
                printf("%ld: ", reg[A]);
                output = runProgram(reg, program);
                printTllint(output);
                if (tll_length(output) == tll_length(program))
                        break;
        }
        // for (a2 = a; a2 < a + ipow(8, progLen); a2 += ipow(8, progLen - 1)) {
        //         reg[A] = a2;
        //         printf("%ld: ", reg[A]);
        //         output = runProgram(reg, program);
        //         printTllint(output);
        //         if (tll_back(output) == tll_back(program))
        //                 break;
        // }
        // printf("****************************************\n");
        // a = findNextDigit(program, output, a, 15);
        // a = findNextDigit(program, output, a, 14);

        // for (int i = progLen - 1; i >= 0; i--) {
        //         int occur = 1;
        //         a = findNextDigit(program, a, i, occur);
        // }
        long aReg = findStartVal(program, a, progLen - 1);
        printf("A Reg: %ld\n", aReg);

        printTllint(program);

        char outStr[tll_length(output)];
        int count = 0;
        tll_foreach(output, num) {
                outStr[count] = '0' + num->item;
                outStr[count + 1] = ',';
                count += 2;
        }
        outStr[tll_length(output) * 2 - 1] = '\0';

        printf("Part 2: Output: %s\n\n", outStr);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/test.txt");
        else
                ll = getInputFile("assets/2024/Day17.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
