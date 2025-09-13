/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Jan 28, 2025 17:14:07 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
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

void bin(uint64_t num) {
        if (num > 1) {
                bin(num / 2);
        }
        printf("%lu", num % 2);
}

int tllintAtIndex(tllint i, int index) {
        int counter = 0;
        tll_foreach(i, it) {
                if (counter == index)
                        return it->item;
                counter++;
        }
        return -1; // if index out of range
}

uint64_t combo(uint64_t reg[3], int operand) {
        uint64_t out;
        if (operand < 4)
                out = operand;
        else if (operand < 7)
                out = reg[operand - 4];
        else
                out = -1;
        return out;
}

void xdv(uint64_t reg[3], int operand, int outReg) {
        uint64_t denominator = combo(reg, operand);
        denominator = ipow(2, denominator);

        uint64_t answer = reg[0] / denominator;
        reg[outReg] = answer;
}

tllint runProgram(uint64_t reg[3], tllint program) {
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
                uint64_t opcode = p[instPointer];
                uint64_t operand = p[instPointer + 1];
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

uint64_t findRegA(tllint prog, int progIndex, uint64_t start) {
        if (progIndex == (int)tll_length(prog))
                return start;

        int goal = tllintAtIndex(prog, progIndex);
        for (uint64_t i = 0; i < 8; i++) {
                uint64_t aReg = start + (i << 7);
                uint64_t reg[] = {aReg, 0, 0};
                tllint out = runProgram(reg, prog);
                int64_t ret = -1;
                if (tll_front(out) == goal &&
                                !(progIndex == (int)tll_length(prog) - 1 &&
                                tll_length(out) != 1)) {
                        ret = findRegA(prog, progIndex + 1, aReg >> 3);
                }
                if (ret != -1) {
                        ret = (ret << 3) | (start % 8);
                        return ret;
                }
        }

        return -1;
}

void part1(llist *ll) {
        uint64_t reg[3] = {0};
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

        uint64_t aReg = 0;
        for (uint64_t i = 0; i < (1 << 10); i++) {
                uint64_t reg[] = {i, 0, 0};
                tllint out = runProgram(reg, program);
                int64_t ret = -1;
                if (tll_front(out) == tll_front(program)) {
                        ret = findRegA(program, 1, i >> 3);
                }
                if (ret != -1) {
                        aReg = (ret << 3) | (i % 8);
                        break;
                }
        }

        uint64_t reg[3] = {0};
        reg[0] = aReg;
        tllint out = runProgram(reg, program);
        printf("Program: ");
        printTllint(program);
        printf("Output:  ");
        printTllint(out);
        printf("Part 2: Reg A: %ld\n\n", aReg);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day17.txt");
        else
                ll = getInputFile("assets/inputs/2024/Day17.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
