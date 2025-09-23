/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Sep 23, 2025 16:44:20 UTC
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
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(char*) tllstr;

struct input {
        tllstr nums;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printNum(char *num) {
        for (int i=0; i<(int)strlen(num); i++) {
                if (num[i] == '(') {
                        printf("[");
                } else if (num[i] == ',') {
                        printf(",");
                } else if (num[i] == ')') {
                        printf("]");
                } else {
                        printf("%d", num[i]-'0');
                }
        }
        printf("\n");
}

char *addNums(char *a, char *b) {
        int len1 = strlen(a);
        int len2 = strlen(b);
        char *add = calloc(len1+len2+4, sizeof(char));

        add[0] = '(';
        strncpy(add+1, a, len1);
        add[len1+1] = ',';
        strncpy(add+len1+2, b, len2);
        add[len1+len2+2] = ')';
        add[len1+len2+3] = '\0';

        return add;
}

// All chars that are not [ ] or , are digits
bool isDigitSF(char c) {
        return (c != '(') && (c != ')') && (c != ',');
}

// Returns NULL if no explosions
char *explodeNum(char *num) {
        int depth = 0;
        for (int i=0; i<(int)strlen(num); i++) {
                if (num[i] == '(') {
                        depth++;
                        continue;
                } if (num[i] == ')') {
                        depth--;
                        continue;
                } if (depth <= 4) {
                        continue;
                }
                // if is reg pair and depth > 4, explode
                if (isDigitSF(num[i]) && isDigitSF(num[i+2])) {
                        // printf("%c, %c\n", num[i], num[i+2]);
                        char *new = calloc(strlen(num)-3, sizeof(char));
                        // Copy left side and add left element to first reg num

                        bool added = false;
                        for (int j=i-2; j>=0; j--) {
                                new[j] = num[j];
                                if (!added && isDigitSF(num[j])) {
                                        // Just use ascii codes after 9
                                        // 10-18 -> :;<=>?@AB
                                        new[j] += num[i] - '0';
                                        added = true;
                                }
                        }
                        new[i-1] = '0';

                        // Copy right side, add right element to first reg num
                        added = false;
                        for (int j=i+4; j<(int)strlen(num); j++) {
                                new[j-4] = num[j];
                                if (!added && isDigitSF(num[j])) {
                                        // Just use ascii codes after 9
                                        // 10-18 -> :;<=>?@AB
                                        new[j-4] += num[i+2] - '0';
                                        added = true;
                                }
                        }
                        new[strlen(num)-3] = '\0';
                        return new;
                }
        }
        return NULL;
}

// Returns NULL if no splits
char *splitNum(char *num) {
        for (int i=0; i<(int)strlen(num); i++) {
                int charNum = num[i] - '0';
                if (!isDigitSF(num[i]) || charNum < 10)
                        continue;

                int numL = charNum / 2;
                int numR = numL;
                if (charNum % 2 != 0)
                        numR++;

                char *new = calloc(strlen(num)+5, sizeof(char));
                strncpy(new, num, i);
                new[i] = '(';
                new[i+1] = numL + '0';
                new[i+2] = ',';
                new[i+3] = numR + '0';
                new[i+4] = ')';
                strncpy(new+i+5, num+i+1, strlen(num)-i);
                return new;
        }
        return NULL;
}

char *reduceNum(char *num) {
        for (;;) {
                // Look for explosions
                char *new = explodeNum(num);
                if (new != NULL) {
                        free(num);
                        num = new;
                        // printNum(num);
                        continue;
                }

                // Look for splits
                new = splitNum(num);
                if (new != NULL) {
                        free(num);
                        num = new;
                        // printNum(num);
                        continue;
                }

                // If no explosions or splits, num is reduced, return
                return num;
        }
}

void reduceMag(int num[], int out[]) {
        int i = 0;
        int j = 0;
        for (;;) {
                if (num[i] == -1) {
                        out[j] = -1; // End of array
                        break;
                }
                if (num[i] >= 0 && num[i+1] >= 0) {
                        // [ 1 2 ] -> 7
                        out[j-1] = (3 * num[i]) + (2 * num[i+1]);
                        i += 2;
                        j--;
                } else {
                        out[j] = num[i];
                }
                i++;
                j++;
        }
}

int32 getMagnitude(char *num) {
        // Convert to int array. '(': -2, ')': -3, '\0': -1
        int len = strlen(num);
        int32 numArr[len];
        int j = 0;
        for (int i=0; i<len; i++) {
                if (num[i] == ',')
                        continue;
                if (num[i] == '(') {
                        numArr[j] = -2;
                } else if (num[i] == ')') {
                        numArr[j] = -3;
                } else {
                        numArr[j] = num[i] - '0';
                }
                j++;
        }
        numArr[j] = -1; // End of array

        // Reduce array until only 1 number
        while (numArr[1] != -1) {
                int32 new[len];
                reduceMag(numArr, new);
                memcpy(numArr, new, len*sizeof(int32));
        }
        return numArr[0];
}

void part1(struct input input) {
        int numNums = tll_length(input.nums);
        char *nums[numNums];

        int i=0;
        tll_foreach(input.nums, it) {
                nums[i] = malloc(strlen(it->item)+1);
                strncpy(nums[i], it->item, strlen(it->item)+1);
                i++;
        }

        char *curr = nums[0];
        for (int i=1; i<numNums; i++) {
                char *add = addNums(curr, nums[i]);
                free(curr);
                curr = reduceNum(add);
                // printNum(curr);
        }

        int32 mag = getMagnitude(curr);

        printf("Part 1: %d\n\n", mag);
}

void part2(struct input input) {
        int numNums = tll_length(input.nums);
        char *nums[numNums];

        int i=0;
        tll_foreach(input.nums, it) {
                nums[i] = malloc(strlen(it->item)+1);
                strncpy(nums[i], it->item, strlen(it->item)+1);
                i++;
        }

        int32 maxMag = 0;
        for (int i=0; i<numNums; i++) {
                for (int j=i+1; j<numNums; j++) {
                        char *add = addNums(nums[i], nums[j]);
                        char *reduced = reduceNum(add);
                        int32 mag = getMagnitude(reduced);
                        if (mag > maxMag)
                                maxMag = mag;

                        add = addNums(nums[j], nums[i]);
                        reduced = reduceNum(add);
                        mag = getMagnitude(reduced);
                        if (mag > maxMag)
                                maxMag = mag;
                }
        }

        printf("Part 2: %d\n", maxMag);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = malloc(BUFFER_SIZE);
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                // Replace [] with () to avoid collisions
                for (int i=0; i<(int)strlen(str); i++) {
                        if (str[i] == '[')
                                str[i] = '(';
                        else if (str[i] == ']')
                                str[i] = ')';
                }
                tll_push_back(input.nums, str);

                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day18.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day18.txt";
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

