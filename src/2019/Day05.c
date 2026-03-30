/*************************************************
 *File----------Day05.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 30, 2026 14:53:37 UTC
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
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tal(int32) talint32;

struct input {
        talint32 intcode;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

talint32 copyList(talint32 list) {
        talint32 newList = tal_init();
        for (int i = 0; i < (int)list.length; i++) {
                tal_add(newList, list.array[i]);
        }
        return newList;
}

int32 getOp(talint32 list, int index) {
        if ((size_t)index < list.length)
                return list.array[index];
        else
                return -1;
}

int32 runIntcode(talint32 intcodeIn, talint32 inputs, talint32 *outputs) {
        talint32 intcode = copyList(intcodeIn);
        int index = 0;
        for (;;) {
                int32 instrNum = intcode.array[index];

                // Instruction Opcode and parameter modes. true: Position, false: Immediate
                int32 opcode = instrNum % 100;
                bool mode1 = ((instrNum / 100) % 10) == 0;
                bool mode2 = ((instrNum / 1000) % 10) == 0;

                // Instruction parameters
                int32 param1 = getOp(intcode, index + 1);
                int32 param2 = getOp(intcode, index + 2);
                int32 param3 = getOp(intcode, index + 3);

                // Instruction values. Obtained with parameters and modes
                int32 op1;
                int32 op2;
                int32 op3;
                int32 nextIndex;

                bool halt = false;
                switch (opcode) {
                case 1: // Addition
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        op3 = param3; // Destinations are always addresses

                        intcode.array[op3] = op1 + op2;
                        index += 4;
                        debugp("(%d) Add: %d %d %d %d: ", index, instrNum, param1, param2, param3);
                        debugp("%d + %d -> %d\n", op1, op2, op3);
                        break;
                case 2: // Multiplication
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        op3 = param3; // Destinations are always addresses

                        intcode.array[op3] = op1 * op2;
                        index += 4;
                        debugp("(%d) Mult: %d %d %d %d: ", index, instrNum, param1, param2, param3);
                        debugp("%d * %d -> %d\n", op1, op2, op3);
                        break;
                case 3: // Input
                        op1 = param1; // Destinations are always addresses
                        intcode.array[op1] = tal_pop_front(inputs);
                        index += 2;
                        debugp("(%d) Input: %d %d\n", index, instrNum, param1);
                        break;
                case 4: // Output
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        tal_add(*outputs, op1);
                        index += 2;
                        debugp("Output: %d %d: %d\n", instrNum, param1, op1);
                        break;
                case 5: // Jump-if-True
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        if (op1 != 0)
                                nextIndex = op2;
                        else
                                nextIndex = index + 3;
                        debugp("(%d) JmpT: %d %d %d: %d -> %d\n",
                                        index, instrNum, param1, param2, op1, nextIndex);
                        index = nextIndex;
                        break;
                case 6: // Jump-if-False
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        if (op1 == 0)
                                nextIndex = op2;
                        else
                                nextIndex = index + 3;
                        debugp("(%d) JmpF: %d %d %d: %d -> %d\n",
                                        index, instrNum, param1, param2, op1, nextIndex);
                        index = nextIndex;
                        break;
                case 7: // Less than
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        op3 = param3; // Destinations are always addresses
                        int32 lt = 0;
                        if (op1 < op2)
                                lt = 1;
                        intcode.array[op3] = lt;
                        index += 4;
                        debugp ("(%d) LT: %d %d %d %d: ", index, instrNum, param1, param2, param3);
                        debugp("%d < %d: %d -> %d\n", op1, op2, lt, op3);
                        break;
                case 8: // Equals
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        op3 = param3; // Destinations are always addresses
                        int32 eq = 0;
                        if (op1 == op2)
                                eq = 1;
                        intcode.array[op3] = eq;
                        index += 4;
                        debugp ("(%d) EQ: %d %d %d %d: ", index, instrNum, param1, param2, param3);
                        debugp("%d == %d: %d -> %d\n", op1, op2, eq, op3);
                        break;
                case 99:
                        halt = true;
                        debugp("Halt\n");
                        break;
                default:
                        printf("Invalid opcode at index %d: %d\n", index, opcode);
                        halt = true;
                }
                if (halt) break;
        }
        int32 output = intcode.array[0];
        tal_destroy(intcode);
        return output;
}

void part1(struct input *input) {
        talint32 intcode = copyList(input->intcode);

        talint32 inputs = tal_init();
        tal_add(inputs, 1); // Only input is 1

        talint32 outputs = tal_init();
        runIntcode(intcode, inputs, &outputs);
        tal_destroy(intcode);

        for (int i = 0; i < (int)outputs.length; i++)
                debugp("%d\n", outputs.array[i]);

        int32 code = outputs.array[outputs.length - 1];

        printf("Part 1: %d\n\n", code);
}

void part2(struct input *input) {
        talint32 intcode = copyList(input->intcode);

        talint32 inputs = tal_init();
        tal_add(inputs, 5); // Only input is 5

        talint32 outputs = tal_init();
        runIntcode(intcode, inputs, &outputs);
        tal_destroy(intcode);

        for (int i = 0; i < (int)outputs.length; i++)
                debugp("%d\n", outputs.array[i]);

        int32 code = outputs.array[outputs.length - 1];

        printf("Part 2: %d\n", code);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *code = strtok(str, ",");
        while (code != NULL) {
                int32 num = strtoul(code, NULL, 10);
                tal_add(input.intcode, num);
                code = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day05.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
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

