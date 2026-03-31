/*************************************************
 *File----------Day09.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 31, 2026 13:57:11 UTC
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

void printIntcode(talint64 intcode) {
        for (int i = 0; i < (int)intcode.length; i++) {
                printf("%ld ", intcode.array[i]);
                if (i % 20 == 19)
                        printf("\n");
        }
        printf("\n");
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

int64 getOp(talint64 list, int64 param, u8 mode, int64 relBase) {
        if (mode == 1)
                return param;

        int64 index;
        if (mode == 0)
                index = param;
        else
                index = param + relBase;

        // Indexes past end of array are zero
        if ((size_t)index < list.length)
                return list.array[index];
        else
                return 0;
}

int64 getDest(int64 param, u8 mode, int64 relBase) {
        if (mode != 2)
                return param;
        return relBase + param;
}

void writeIntcode(talint64 *intcode, int64 index, int64 value) {
        // Index already exixts in array
        if (index < (int64)intcode->length){
                intcode->array[index] = value;
                return;
        }

        // Index can fit in current array capacity
        u64 curLen = intcode->length;
        if (index < (int64)intcode->capicity) {
                intcode->length = index + 1;
                // Set new elements to zero
                for (int i = curLen; i <= index; i++)
                        intcode->array[i] = 0;
                intcode->array[index] = value;
                return;
        }

        // Index does not fit in current array capacity, extend the array and fill with zeros
        tal_allocate(*intcode, (u64)index + 1);
        intcode->length = index + 1;
        for (int i = curLen; i <= index; i++)
                intcode->array[i] = 0;
        intcode->array[index] = value;
}

haltmode runIntcode(talint64 *intcode, int start, talint64 *inputs, talint64 *outputs) {
        int64 index = start;
        int64 relBase = 0;
        bool halt = false;
        bool pause = false;
        for (;;) {
                int64 instrNum = intcode->array[index];

                // Instruction Opcode and parameter modes. true: Position, false: Immediate
                int64 opcode = instrNum % 100;
                u8 mode1 = (instrNum / 100) % 10;
                u8 mode2 = (instrNum / 1000) % 10;
                u8 mode3 = (instrNum / 10000) % 10;

                // Instruction parameters
                int64 param1 = getOp(*intcode, index + 1, 0, 0);
                int64 param2 = getOp(*intcode, index + 2, 0, 0);
                int64 param3 = getOp(*intcode, index + 3, 0, 0);

                // Instruction values. Obtained with parameters and modes
                int64 op1;
                int64 op2;
                int64 op3;
                int64 nextIndex;

                switch (opcode) {
                case 1: // Addition
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        op3 = (mode3 == 2) ? relBase + param3 : param3;

                        writeIntcode(intcode, op3, op1 + op2);
                        debugp("(%ld) Add: %ld %ld %ld %ld: ",
                                        index, instrNum, param1, param2, param3);
                        debugp("%ld + %ld -> %ld\n", op1, op2, op3);
                        index += 4;
                        break;
                case 2: // Multiplication
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        op3 = (mode3 == 2) ? relBase + param3 : param3;

                        writeIntcode(intcode, op3, op1 * op2);
                        debugp("(%ld) Mult: %ld %ld %ld %ld: ",
                                        index, instrNum, param1, param2, param3);
                        debugp("%ld * %ld -> %ld\n", op1, op2, op3);
                        index += 4;
                        break;
                case 3: // Input
                        op1 = (mode1 == 2) ? relBase + param1 : param1;
                        if (inputs->length == 0) {
                                pause = true;
                                debugp("(%ld) Input: %ld %ld: PAUSE\n", index, instrNum, param1);
                                break;
                        }
                        writeIntcode(intcode, op1, tal_pop_front(*inputs));
                        debugp("(%ld) Input: %ld %ld: ", index, instrNum, param1);
                        debugp("%ld -> %ld\n", intcode->array[op1], op1);
                        index += 2;
                        break;
                case 4: // Output
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        tal_add(*outputs, op1);
                        debugp("(%ld)Output: %ld %ld: %ld\n", index, instrNum, param1, op1);
                        index += 2;
                        break;
                case 5: // Jump-if-True
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        if (op1 != 0)
                                nextIndex = op2;
                        else
                                nextIndex = index + 3;
                        debugp("(%ld) JmpT: %ld %ld %ld: %ld -> %ld\n",
                                        index, instrNum, param1, param2, op1, nextIndex);
                        index = nextIndex;
                        break;
                case 6: // Jump-if-False
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        if (op1 == 0)
                                nextIndex = op2;
                        else
                                nextIndex = index + 3;
                        debugp("(%ld) JmpF: %ld %ld %ld: %ld -> %ld\n",
                                        index, instrNum, param1, param2, op1, nextIndex);
                        index = nextIndex;
                        break;
                case 7: // Less than
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        op3 = (mode3 == 2) ? relBase + param3 : param3;
                        int64 lt = 0;
                        if (op1 < op2)
                                lt = 1;
                        writeIntcode(intcode, op3, lt);
                        debugp ("(%ld) LT: %ld %ld %ld %ld: ",
                                        index, instrNum, param1, param2, param3);
                        debugp("%ld < %ld: %ld -> %ld\n", op1, op2, lt, op3);
                        index += 4;
                        break;
                case 8: // Equals
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        op3 = (mode3 == 2) ? relBase + param3 : param3;
                        int64 eq = 0;
                        if (op1 == op2)
                                eq = 1;
                        writeIntcode(intcode, op3, eq);
                        debugp ("(%ld) EQ: %ld %ld %ld %ld: ",
                                        index, instrNum, param1, param2, param3);
                        debugp("%ld == %ld: %ld -> %ld\n", op1, op2, eq, op3);
                        index += 4;
                        break;
                case 9: // Move Relative Base
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        relBase += op1;
                        debugp("(%ld)Move RelBase: %ld %ld: %ld\n", index, instrNum, param1, op1);
                        index += 2;
                        break;
                case 99:
                        halt = true;
                        debugp("(%ld) Halt\n", index);
                        break;
                default:
                        printf("Invalid opcode at index %ld: %ld\n", index, opcode);
                        halt = true;
                }
                if (halt || pause) break;
        }
        haltmode h = {halt, index};
        return h;
}

void part1(struct input *input) {
        talint64 intcode = copyList(input->intcode);
        talint64 inputs = tal_init();
        talint64 outputs = tal_init();

        tal_add(inputs, 1); // Provided input

        runIntcode(&intcode, 0, &inputs, &outputs);
        tal_destroy(intcode);

        int64 keycode = tal_back(outputs);

        printf("Part 1: %ld\n\n", keycode);
}

void part2(struct input *input) {
        talint64 intcode = copyList(input->intcode);
        talint64 inputs = tal_init();
        talint64 outputs = tal_init();

        tal_add(inputs, 2); // Provided input

        runIntcode(&intcode, 0, &inputs, &outputs);

        int64 coordinates = tal_back(outputs);
        printf("Part 2: %ld\n", coordinates);
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
                char *file = "assets/tests/2019/Day09.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day09.txt";
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

