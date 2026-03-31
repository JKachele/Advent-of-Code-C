/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 30, 2026 17:03:51 UTC
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

typedef tal(int64) talint64;

typedef struct haltmode {
        bool halt; // True: halt, False: Pause for input
        u32 index;
} haltmode;

struct input {
        talint64 intcode;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

talint64 copyList(talint64 list) {
        talint64 newList = tal_init();
        for (int i = 0; i < (int)list.length; i++) {
                tal_add(newList, list.array[i]);
        }
        return newList;
}

void copyList2(talint64 list, talint64 *newList) {
        for (int i = 0; i < (int)list.length; i++) {
                tal_add(*newList, list.array[i]);
        }
}

void MoveListItems(talint64 *dest, talint64 *src) {
        while (src->length > 0) {
                tal_add(*dest, tal_pop_front(*src));
        }
}

int64 getOp(talint64 list, int index) {
        if ((size_t)index < list.length)
                return list.array[index];
        else
                return -1;
}

haltmode runIntcode(talint64 intcode, int start, talint64 *inputs, talint64 *outputs) {
        int index = start;
        bool halt = false;
        bool pause = false;
        for (;;) {
                int64 instrNum = intcode.array[index];

                // Instruction Opcode and parameter modes. true: Position, false: Immediate
                int64 opcode = instrNum % 100;
                bool mode1 = ((instrNum / 100) % 10) == 0;
                bool mode2 = ((instrNum / 1000) % 10) == 0;

                // Instruction parameters
                int64 param1 = getOp(intcode, index + 1);
                int64 param2 = getOp(intcode, index + 2);
                int64 param3 = getOp(intcode, index + 3);

                // Instruction values. Obtained with parameters and modes
                int64 op1;
                int64 op2;
                int64 op3;
                int64 nextIndex;

                switch (opcode) {
                case 1: // Addition
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        op3 = param3; // Destinations are always addresses

                        intcode.array[op3] = op1 + op2;
                        debugp("(%d) Add: %d %d %d %d: ", index, instrNum, param1, param2, param3);
                        debugp("%d + %d -> %d\n", op1, op2, op3);
                        index += 4;
                        break;
                case 2: // Multiplication
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        op3 = param3; // Destinations are always addresses

                        intcode.array[op3] = op1 * op2;
                        debugp("(%d) Mult: %d %d %d %d: ", index, instrNum, param1, param2, param3);
                        debugp("%d * %d -> %d\n", op1, op2, op3);
                        index += 4;
                        break;
                case 3: // Input
                        op1 = param1; // Destinations are always addresses
                        if (inputs->length == 0) {
                                pause = true;
                                debugp("(%d) Input: %d %d: PAUSE\n", index, instrNum, param1);
                                break;
                        }
                        intcode.array[op1] = tal_pop_front(*inputs);
                        debugp("(%d) Input: %d %d: ", index, instrNum, param1);
                        debugp("%d -> %d\n", intcode.array[op1], op1);
                        index += 2;
                        break;
                case 4: // Output
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        tal_add(*outputs, op1);
                        debugp("Output: %d %d: %d\n", instrNum, param1, op1);
                        index += 2;
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
                        int64 lt = 0;
                        if (op1 < op2)
                                lt = 1;
                        intcode.array[op3] = lt;
                        debugp ("(%d) LT: %d %d %d %d: ", index, instrNum, param1, param2, param3);
                        debugp("%d < %d: %d -> %d\n", op1, op2, lt, op3);
                        index += 4;
                        break;
                case 8: // Equals
                        op1 = mode1 ? getOp(intcode, param1) : param1;
                        op2 = mode2 ? getOp(intcode, param2) : param2;
                        op3 = param3; // Destinations are always addresses
                        int64 eq = 0;
                        if (op1 == op2)
                                eq = 1;
                        intcode.array[op3] = eq;
                        debugp ("(%d) EQ: %d %d %d %d: ", index, instrNum, param1, param2, param3);
                        debugp("%d == %d: %d -> %d\n", op1, op2, eq, op3);
                        index += 4;
                        break;
                case 99:
                        halt = true;
                        debugp("(%d) Halt\n", index);
                        break;
                default:
                        printf("Invalid opcode at index %d: %ld\n", index, opcode);
                        halt = true;
                }
                if (halt || pause) break;
        }
        haltmode h = {halt, index};
        return h;
}

void swap(int *a, int *b) {
        int tmp = *a;
        *a = *b;
        *b = tmp;
}

u64 getThrust(talint64 intcode, int phaseSettings[5]) {
        u32 currentInput = 0;
        for (int i = 0; i < 5; i++) {
                talint64 inputs = tal_init();
                talint64 outputs = tal_init();
                tal_add(inputs, phaseSettings[i]);
                tal_add(inputs, currentInput);

                talint64 copy = copyList(intcode);
                runIntcode(copy, 0, &inputs, &outputs);
                tal_destroy(copy);
                currentInput = tal_back(outputs);

                tal_destroy(inputs);
                tal_destroy(outputs);
        }
        return currentInput;
}

u64 getThrustLoop(talint64 intcode, int phaseSettings[5]) {
        talint64 memory[5] = {0};
        talint64 inputs[5] = {0};
        talint64 outputs[5] = {0};
        haltmode halts[5] = {0};
        for (int i = 0; i < 5; i++) {
                tal_add(inputs[i], phaseSettings[i]);
                copyList2(intcode, &memory[i]);
        }
        tal_add(inputs[0], 0);

        for (;;) {
                bool allHalt = true;
                for (int i = 0; i < 5; i++) {
                        debugp("**************** AMP %d ****************\n", i);
                        halts[i] = runIntcode(memory[i], halts[i].index, &inputs[i], &outputs[i]);
                        MoveListItems(&inputs[(i+1)%5], &outputs[i]);
                        if (!halts[i].halt)
                                allHalt = false;
                }
                if (allHalt)
                        break;
        }

        return tal_back(inputs[0]);
}

u64 getMaxThrust(talint64 intcode, int phases[], int l, int r, bool part1) {
        static u64 maxThrust = 0;
        if (l == r) {
                u64 thrust = part1 ? getThrust(intcode, phases) : getThrustLoop(intcode, phases);
                if (thrust > maxThrust)
                        maxThrust = thrust;
        } else {
                for (int i = l; i <= r; i++) {
                        swap(&phases[l], &phases[i]);
                        u64 thrust = getMaxThrust(intcode, phases, l + 1, r, part1);
                        if (thrust > maxThrust)
                                maxThrust = thrust;

                        // Backtrack
                        swap(&phases[l], &phases[i]);
                }
        }

        return maxThrust;
}

void part1(struct input *input) {
        int phases[] = {0, 1, 2, 3, 4};
        u64 maxThrust = getMaxThrust(input->intcode, phases, 0, 4, true);

        printf("Part 1: %lu\n\n", maxThrust);
}

void part2(struct input *input) {
        int phases[] = {5,6,7,8,9};
        u64 maxThrust = getMaxThrust(input->intcode, phases, 0, 4, false);

        printf("Part 2: %lu\n", maxThrust);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *code = strtok(str, ",");
        while (code != NULL) {
                int64 num = strtoul(code, NULL, 10);
                tal_add(input.intcode, num);
                code = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day07.txt";
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

