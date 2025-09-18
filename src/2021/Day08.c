/*************************************************
 *File----------Day08.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 17, 2025 17:41:46 UTC
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

typedef struct digitkey {
        char digits[10][8];
} digitkey;

typedef struct entry {
        char digits[10][8];
        char output[4][8];
} entry;
typedef tll(entry) tllentry;

struct input {
        tllentry log;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

// Decode digits 2, 5, and 6
void decode256(digitkey *key, tllstr *unknown) {
        // These only have 1 of the 2 segments of 1
        char *one = key->digits[1];
        tllstr twofivesix = tll_init();
        tll_foreach(*unknown, it) {
                char *digit = it->item;
                if (strchr(digit, one[0]) != NULL &&
                                strchr(digit, one[1]) == NULL) {
                        tll_push_back(twofivesix, digit);
                        tll_remove(*unknown, it);
                }
                if (strchr(digit, one[0]) == NULL &&
                                strchr(digit, one[1]) != NULL) {
                        tll_push_back(twofivesix, digit);
                        tll_remove(*unknown, it);
                }
        }
        // 6 has 6 segments, 2 and 5 only have 5
        tll_foreach(twofivesix, it) {
                if (strlen(it->item) == 6) {
                        strcpy(key->digits[6], it->item);
                        tll_remove(twofivesix, it);
                        break;
                }
        }
        // The segment in both 1 and 6 is segment F
        char segF;
        if (strchr(key->digits[6], one[0]) != NULL)
                segF = one[0];
        else
                segF = one[1];
        // 5 has segment F, 2 does not
        if (strchr(tll_front(twofivesix), segF) != NULL) {
                strcpy(key->digits[5], tll_front(twofivesix));
                strcpy(key->digits[2], tll_back(twofivesix));
        } else {
                strcpy(key->digits[2], tll_front(twofivesix));
                strcpy(key->digits[5], tll_back(twofivesix));
        }
        tll_free(twofivesix);
}

// Decode digits 0 and 9
void decode09(digitkey *key, tllstr *unknown) {
        // 9 has all the segments of 4 while 0 does not
        char *four = key->digits[4];
        char *unknown1 = tll_front(*unknown);
        bool is9 = true;
        for (int i=0; i<4; i++) {
                if (strchr(unknown1, four[i]) == NULL) {
                        is9 = false;
                        break;
                }
        }
        if (is9) {
                strcpy(key->digits[9], unknown1);
                strcpy(key->digits[0], tll_back(*unknown));
        } else {
                strcpy(key->digits[0], unknown1);
                strcpy(key->digits[9], tll_back(*unknown));
        }
}

digitkey decodeDisplay(entry e) {
        digitkey key = {0};
        tllstr unknown = tll_init();

        // Decode 4 known digits: 1, 4, 7, and 8
        for (int i=0; i<10; i++) {
                char *digit = e.digits[i];
                switch (strlen(digit)) {
                case 2:
                        strcpy(key.digits[1], digit);
                        break;
                case 3:
                        strcpy(key.digits[7], digit);
                        break;
                case 4:
                        strcpy(key.digits[4], digit);
                        break;
                case 7:
                        strcpy(key.digits[8], digit);
                        break;
                default:
                        tll_push_back(unknown, digit);
                }
        }

        decode256(&key, &unknown);

        // Of the remaining digits(0, 3, 9), only 3 has 5 segments
        tll_foreach(unknown, it) {
                if (strlen(it->item) == 5) {
                        strcpy(key.digits[3], it->item);
                        tll_remove(unknown, it);
                }
        }

        decode09(&key, &unknown);
        tll_free(unknown);

        return key;
}

bool isSameDigit(char *out, char *digit) {
        if (strlen(out) != strlen(digit))
                return false;
        bool isDigit = true;
        for (int i=0; i<(int)strlen(digit); i++) {
                if (strchr(out, digit[i]) == NULL) {
                        isDigit = false;
                        break;
                }
        }
        return isDigit;
}

int32 extractOutput(entry e, digitkey key) {
        int32 output = 0;
        for (int i=0; i<4; i++) {
                output *= 10;
                char *outSeg = e.output[i];
                for (int j=0; j<10; j++) {
                        if (isSameDigit(outSeg, key.digits[j])) {
                                output += j;
                                break;
                        }
                }
        }
        return output;
}

void part1(struct input input) {
        tllentry log = input.log;

        int32 numUnique = 0;
        tll_foreach(log, it) {
                for (int i=0; i<4; i++) {
                        char *output = it->item.output[i];
                        int32 len = strlen(output);
                        if (len == 2 || len == 3 || len == 4 || len == 7)
                                numUnique++;
                }
        }

        printf("Part 1: %d\n\n", numUnique);
}

void part2(struct input input) {
        tllentry log = input.log;

        int32 outSum = 0;
        tll_foreach(log, it) {
                digitkey key = decodeDisplay(it->item);
                int32 output = extractOutput(it->item, key);
                outSum += output;
                // printf("%04d\n", output);
        }

        printf("Part 2: %d\n", outSum);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        tllentry log = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;

                entry e = {0};

                char *token = strtok(str, " ");
                for (int i=0; i<10; i++) {
                        memcpy(e.digits[i], token, strlen(token));
                        token = strtok(NULL, " ");
                }
                token = strtok(NULL, " ");
                for (int i=0; i<4; i++) {
                        memcpy(e.output[i], token, strlen(token));
                        token = strtok(NULL, " ");
                }
                tll_push_back(log, e);

                current = current->next;
        }
        input.log = log;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day08.txt";
                // char *file = "assets/tests/2021/Day08b.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day08.txt";
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

