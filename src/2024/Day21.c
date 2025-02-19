/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Feb 13, 2025 09:41:06 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

/*
 * +---+---+---+        +---+---+        +---+---+   
 * | 7 | 8 | 9 |        | ^ | A |        | ^ | A |   
 * +---+---+---+    +---+---+---+    +---+---+---+   
 * | 4 | 5 | 6 |    | < | v | > |    | < | v | > |   
 * +---+---+---+    +---+---+---+    +---+---+---+   
 * | 1 | 2 | 3 |                   
 * +---+---+---+                   
 *     | 0 | A |                   
 *     +---+---+                   
 */

#define NUM_CODES 5
#define CODE_LENGTH 4

typedef tll(int8) tllint8;

typedef struct {
        int8 x;
        int8 y;
        char key;
} key;

const key NUM_PAD[11] = {
        {1, 3, '0'},
        {0, 2, '1'},
        {1, 2, '2'},
        {2, 2, '3'},
        {0, 1, '4'},
        {1, 1, '5'},
        {2, 1, '6'},
        {0, 0, '7'},
        {1, 0, '8'},
        {2, 0, '9'},
        {2, 3, 'A'}
};
#define NUM_A 10

const key DIR_PAD[5] = {
        {1, 0, '^'},
        {2, 1, '>'},
        {1, 1, 'v'},
        {0, 1, '<'},
        {2, 0, 'A'}
};
#define DIR_U 0
#define DIR_R 1
#define DIR_D 2
#define DIR_L 3
#define DIR_A 4
#define N_DIR_KEYS 5

#define RL true
#define UD false

void printSeq(int8 code[CODE_LENGTH], tllint8 seq) {
        for (int i = 0; i < CODE_LENGTH; i++) {
                printf("%c", NUM_PAD[code[i]].key);
        }
        printf(": ");
        tll_foreach(seq, it) {
                int8 key = it->item;
                printf("%c", DIR_PAD[key].key);
        }
        printf(" - %zu\n", tll_length(seq));
}

int16 arrToInt(int8 arr[], int8 len) {
        int16 out = 0;
        int16 mult = 1;
        for (int i = len - 1; i >= 0; i--) {
                out += arr[i] * mult;
                mult *= 10;
        }
        return out;
}

void addToSeq(tllint8 *seq, int8 numToAdd, bool rl) {
        for (int i = 0; i < numToAdd; i++) {
                int8 dir = rl ? DIR_R : DIR_D;
                tll_push_back(*seq, dir);
        }
        for (int i = 0; i > numToAdd; i--) {
                int8 dir = rl ? DIR_L : DIR_U;
                tll_push_back(*seq, dir);
        }
}

// Greedy algorithm: Avoid the empty space
// If moving left (dx < 0) move in x then y, else move in y then x
tllint8 getNumPadSeq(int8 code[CODE_LENGTH]) {
        tllint8 seq = tll_init();
        int8 x = NUM_PAD[NUM_A].x;
        int8 y = NUM_PAD[NUM_A].y;
        for (int i = 0; i < CODE_LENGTH; i++) {
                int8 num = code[i];
                int8 newX = NUM_PAD[num].x;
                int8 newY = NUM_PAD[num].y;
                int8 dx = newX - x;
                int8 dy = newY - y;
                // Avoid Empty Space
                if (x == 0 && newY == 3) { // Avoid Empty Space
                        addToSeq(&seq, dx, RL);
                        addToSeq(&seq, dy, UD);
                } else if (y == 3 && newX == 0) { // Avoid Empty Space
                        addToSeq(&seq, dy, UD);
                        addToSeq(&seq, dx, RL);
                } else if (dx < 0) { // Moving left: Move in X then in Y
                        addToSeq(&seq, dx, RL);
                        addToSeq(&seq, dy, UD);
                } else { // Moving right: Move in Y then in X
                        addToSeq(&seq, dy, UD);
                        addToSeq(&seq, dx, RL);
                }
                tll_push_back(seq, DIR_A);
                x = newX;
                y = newY;
        }
        return seq;
}

tllint8 getDirPadSeq(tllint8 inSeq) {
        tllint8 seq = tll_init();
        int8 x = DIR_PAD[DIR_A].x;
        int8 y = DIR_PAD[DIR_A].y;
        tll_foreach(inSeq, it) {
                int8 key = it->item;
                int8 newX = DIR_PAD[key].x;
                int8 newY = DIR_PAD[key].y;
                int8 dx = newX - x;
                int8 dy = newY - y;
                // Avoid Empty Space
                if (x == 0 && newY == 0) { // Avoid Empty Space
                        addToSeq(&seq, dx, RL);
                        addToSeq(&seq, dy, UD);
                } else if (y == 0 && newX == 0) { // Avoid Empty Space
                        addToSeq(&seq, dy, UD);
                        addToSeq(&seq, dx, RL);
                } else if (dx < 0) { // Moving left: Move in X then in Y
                        addToSeq(&seq, dx, RL);
                        addToSeq(&seq, dy, UD);
                } else { // Moving right: Move in Y then in X
                        addToSeq(&seq, dy, UD);
                        addToSeq(&seq, dx, RL);
                }
                tll_push_back(seq, DIR_A);
                x = newX;
                y = newY;
        }
        return seq;
}

uint64 getDirPadCount(uint64 cache[][N_DIR_KEYS][N_DIR_KEYS], int8 numLevels,
                int8 curKey, int8 nextKey, int level) {
        if (level >= numLevels) return -1;
        uint64 count = 0;
        uint64 cacheVal = cache[level][curKey][nextKey];
        if (cacheVal != UINT64_MAX) return cacheVal;
        if (level == 0) return 1;

        int8 x = DIR_PAD[curKey].x;
        int8 y = DIR_PAD[curKey].y;
        int8 newX = DIR_PAD[nextKey].x;
        int8 newY = DIR_PAD[nextKey].y;
        int8 dx = newX - x;
        int8 dy = newY - y;

        tllint8 seq = tll_init();
        // Avoid Empty Space
        if (x == 0 && newY == 0) { // Avoid Empty Space
                addToSeq(&seq, dx, RL);
                addToSeq(&seq, dy, UD);
        } else if (y == 0 && newX == 0) { // Avoid Empty Space
                addToSeq(&seq, dy, UD);
                addToSeq(&seq, dx, RL);
        } else if (dx < 0) { // Moving left: Move in X then in Y
                addToSeq(&seq, dx, RL);
                addToSeq(&seq, dy, UD);
        } else { // Moving right: Move in Y then in X
                addToSeq(&seq, dy, UD);
                addToSeq(&seq, dx, RL);
        }
        tll_push_back(seq, DIR_A);

        // Recursive step to get all controllers
        int8 key = DIR_A;
        tll_foreach(seq, it) {
                int8 next = it->item;
                count += getDirPadCount(cache, numLevels, key, next, level - 1);
                key = next;
        }
        cache[level][curKey][nextKey] = count;
        return count;
}

void initCache(uint64 cache[][N_DIR_KEYS][N_DIR_KEYS], int8 numLevels) {
        MAKE_LOOP(i, numLevels, j, N_DIR_KEYS, k, N_DIR_KEYS) {
                cache[i][j][k] = UINT64_MAX;
        }
}

void part1(llist *ll) {
        int8 codes[NUM_CODES][CODE_LENGTH];

        llNode *current = ll->head;
        for (int i = 0; i < NUM_CODES; i++) {
                char str[BUFFER_SIZE + 1];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int j = 0; j < CODE_LENGTH - 1; j++) {
                        codes[i][j] = str[j] - '0';
                }
                codes[i][CODE_LENGTH - 1] = NUM_A;
                current = current->next;
        }

        uint64 complexity = 0;
        for (int i = 0; i < NUM_CODES; i++) {
                tllint8 numSeq = getNumPadSeq(codes[i]);
                // printSeq(codes[i], numSeq);
                tllint8 dir1Seq = getDirPadSeq(numSeq);
                // printSeq(codes[i], dir1Seq);
                tllint8 dir2Seq = getDirPadSeq(dir1Seq);
                // printSeq(codes[i], dir2Seq);
                int16 code = arrToInt(codes[i], CODE_LENGTH - 1);
                complexity += tll_length(dir2Seq) * code;
        }

        printf("Part 1: Complexity = %lu\n\n", complexity);
}

void part2(llist *ll) {
        const int8 N_KEY_PADS = 26;
        int8 codes[NUM_CODES][CODE_LENGTH];

        llNode *current = ll->head;
        for (int i = 0; i < NUM_CODES; i++) {
                char str[BUFFER_SIZE + 1];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int j = 0; j < CODE_LENGTH - 1; j++) {
                        codes[i][j] = str[j] - '0';
                }
                codes[i][CODE_LENGTH - 1] = NUM_A;
                current = current->next;
        }

        uint64 cache[N_KEY_PADS][N_DIR_KEYS][N_DIR_KEYS];
        initCache(cache, N_KEY_PADS);

        uint64 complexity = 0;
        for (int i = 0; i < NUM_CODES; i++) {
                tllint8 numSeq = getNumPadSeq(codes[i]);
                int8 key = DIR_A;
                uint64 count = 0;
                tll_foreach(numSeq, it) {
                        int8 next = it->item;
                        count += getDirPadCount(cache, N_KEY_PADS,
                                        key, next, N_KEY_PADS - 1);
                        key = next;
                }
                int16 code = arrToInt(codes[i], CODE_LENGTH - 1);
                complexity += count * code;
        }

        printf("Part 2: Complexity = %lu\n\n", complexity);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day21.txt");
        } else {
                ll = getInputFile("assets/2024/Day21.txt");
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
