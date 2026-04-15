/*************************************************
 *File----------Intcode2019.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 02, 2026 14:42:17 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include "Intcode2019.h"
#include <stdio.h>
#include <stdarg.h>
#include "util.h"
#include "talist.h"

bool IntcodeVerbose = false;
static void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (IntcodeVerbose)
                vprintf(format, args);
        va_end(args);
}

static int64 getOp(talint64 list, int64 param, u8 mode, int64 relBase) {
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

static void writeIntcode(talint64 *intcode, int64 index, int64 value) {
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

talint64 copyIntcode(talint64 intcode) {
        talint64 newList = tal_init();
        for (int i = 0; i < (int)intcode.length; i++) {
                tal_add(newList, intcode.array[i]);
        }
        return newList;
}

void copyIntcodePtr(talint64 *dest, talint64 intcode) {
        for (int i = 0; i < (int)intcode.length; i++) {
                tal_add(*dest, intcode.array[i]);
        }
}

haltmode runIntcode(talint64 *intcode, haltmode state, talint64 *inputs, talint64 *outputs) {
        u32 index = state.index;
        int64 relBase = state.relBase;
        bool halt = false;
        bool pause = false;
        for (;;) {
                int64 instrNum = intcode->array[index];
                // printf("%ld\n", index);

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

                        debugp("(%u) Add: %ld %ld %ld %ld: ",
                                        index, instrNum, param1, param2, param3);
                        debugp("%ld + %ld -> %ld\n", op1, op2, op3);
                        writeIntcode(intcode, op3, op1 + op2);
                        index += 4;
                        break;
                case 2: // Multiplication
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        op3 = (mode3 == 2) ? relBase + param3 : param3;

                        debugp("(%u) Mult: %ld %ld %ld %ld: ",
                                        index, instrNum, param1, param2, param3);
                        debugp("%ld * %ld -> %ld\n", op1, op2, op3);
                        writeIntcode(intcode, op3, op1 * op2);
                        index += 4;
                        break;
                case 3: // Input
                        op1 = (mode1 == 2) ? relBase + param1 : param1;
                        if (inputs->length == 0) {
                                pause = true;
                                debugp("PAUSE: (%u) Input: %ld %ld\n", index, instrNum, param1);
                                break;
                        }
                        int64 in = tal_pop_front(*inputs);
                        debugp("(%u) Input: %ld %ld: ", index, instrNum, param1);
                        debugp("%ld -> %ld\n", intcode->array[op1], op1);
                        writeIntcode(intcode, op1, in);
                        index += 2;
                        break;
                case 4: // Output
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        tal_add(*outputs, op1);
                        debugp("(%u) Output: %ld %ld: %ld\n", index, instrNum, param1, op1);
                        index += 2;
                        break;
                case 5: // Jump-if-True
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        if (op1 != 0)
                                nextIndex = op2;
                        else
                                nextIndex = index + 3;
                        debugp("(%u) JmpT: %ld %ld %ld: %ld -> %ld\n",
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
                        debugp("(%u) JmpF: %ld %ld %ld: %ld -> %ld\n",
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
                        debugp ("(%u) LT: %ld %ld %ld %ld: ",
                                        index, instrNum, param1, param2, param3);
                        debugp("%ld < %ld: %ld -> %ld\n", op1, op2, lt, op3);
                        writeIntcode(intcode, op3, lt);
                        index += 4;
                        break;
                case 8: // Equals
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        op2 = getOp(*intcode, param2, mode2, relBase);
                        op3 = (mode3 == 2) ? relBase + param3 : param3;
                        int64 eq = 0;
                        if (op1 == op2)
                                eq = 1;
                        debugp ("(%u) EQ: %ld %ld %ld %ld: ",
                                        index, instrNum, param1, param2, param3);
                        debugp("%ld == %ld: %ld -> %ld\n", op1, op2, eq, op3);
                        writeIntcode(intcode, op3, eq);
                        index += 4;
                        break;
                case 9: // Move Relative Base
                        op1 = getOp(*intcode, param1, mode1, relBase);
                        relBase += op1;
                        debugp("(%u) Move RelBase: %ld %ld: %ld\n", index, instrNum, param1, op1);
                        index += 2;
                        break;
                case 99:
                        halt = true;
                        debugp("(%u) Halt\n", index);
                        break;
                default:
                        printf("Invalid opcode at index %u: %ld\n", index, opcode);
                        halt = true;
                }
                if (halt || pause) break;
        }
        haltmode h = {halt, index, relBase};
        return h;
}
