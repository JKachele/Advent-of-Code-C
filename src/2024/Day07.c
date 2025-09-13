/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Dec 19, 2024 08:41:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

int MAX_NUMS = 0;

int64_t concat(int64_t a, int64_t b) {
        int64_t pow = 10;
        while (b >= pow)
                pow *= 10;
        return a * pow + b;
}

bool isValid1(int64_t nums[], int64_t testValue, int64_t curAns, int index) {
        if (index >= MAX_NUMS || nums[index] == -1)
                return curAns == testValue;

        if (isValid1(nums, testValue, curAns + nums[index], index + 1)) {
                return true;
        } else {
                return isValid1(nums, testValue,
                                curAns * nums[index], index + 1);
        }
}

bool isValid2(int64_t nums[], int64_t testValue, int64_t curAns, int index) {
        if (index >= MAX_NUMS || nums[index] == -1)
                return curAns == testValue;

        if (isValid2(nums, testValue,
                                curAns + nums[index], index + 1)) {
                return true;
        } else if (isValid2(nums, testValue,
                                curAns * nums[index], index + 1)) {
                return true;
        } else {
                int64_t cat = concat(curAns, nums[index]);
                return isValid2(nums, testValue, cat, index + 1);
        }
}

void part1(llist *ll) {
        llNode *current = ll->head;
        MAX_NUMS = getLongestLine(ll) / 2;
        int64_t testvalues[ll->length];
        int64_t nums[ll->length][MAX_NUMS];

        int lineNum = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                testvalues[lineNum] = 
                        strtol(strtok(str, ":"), (char**)NULL, 10);

                char *numStr = strtok(NULL, " ");
                int numN = 0;
                while (numStr != NULL) {
                        nums[lineNum][numN] = strtol(numStr, (char**)NULL, 10);
                        numStr = strtok(NULL, " ");
                        numN++;
                }
                nums[lineNum][numN] = -1;

                /* printf("%ld: ", testvalues[lineNum]);
                for (int i = 0; i < maxNums; i++) {
                        if (nums[lineNum][i] == -1)
                                break;
                        printf("[%d]", nums[lineNum][i]);
                }
                printf("\n"); */
                current = current->next;
                lineNum++;
        }

        int64_t total = 0;
        for (int i = 0; i < ll->length; i++) {
                if (isValid1(nums[i], testvalues[i], nums[i][0], 1))
                        total += testvalues[i];
        }

        printf("Part 1: Total Calibration Result: %ld\n\n", total);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        MAX_NUMS = getLongestLine(ll) / 2;
        int64_t testvalues[ll->length];
        int64_t nums[ll->length][MAX_NUMS];

        int lineNum = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                testvalues[lineNum] = 
                        strtol(strtok(str, ":"), (char**)NULL, 10);

                char *numStr = strtok(NULL, " ");
                int numN = 0;
                while (numStr != NULL) {
                        nums[lineNum][numN] = strtol(numStr, (char**)NULL, 10);
                        numStr = strtok(NULL, " ");
                        numN++;
                }
                nums[lineNum][numN] = -1;

                /* printf("%ld: ", testvalues[lineNum]);
                for (int i = 0; i < maxNums; i++) {
                        if (nums[lineNum][i] == -1)
                                break;
                        printf("[%d]", nums[lineNum][i]);
                }
                printf("\n"); */
                current = current->next;
                lineNum++;
        }

        int64_t total = 0;
        for (int i = 0; i < ll->length; i++) {
                if (isValid2(nums[i], testvalues[i], nums[i][0], 1)) {
                        total += testvalues[i];
                } else {
                }
        }

        printf("Part 2: Total Calibration Result: %ld\n\n", total);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day07.txt");
        else
                ll = getInputFile("assets/inputs/2024/Day07.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
