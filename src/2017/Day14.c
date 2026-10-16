/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 15, 2026 17:05:50 EDT
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
#include "../util/vector.h"
// #include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024
#define LOOP_SIZE 256
#define GRID_SIZE 128

struct input {
        char *key;
        u64 msb[128];
        u64 lsb[128];
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void stepHash(u8 loop[], u8 length, u8 pos) {
        if (length == 0) return;

        // Select section to reverse
        u8 rev[length];
        for (u8 i = 0; i < length; i++) {
                u8 loopI = (pos + i) % LOOP_SIZE;
                rev[i] = loop[loopI];
        }

        // Reverse section
        u8 f = 0;
        u8 b = length - 1;
        while (f < b) {
                // swap front and back
                u8 temp = rev[f];
                rev[f] = rev[b];
                rev[b] = temp;
                f++;
                b--;
        }

        // Replace loop section with reversed
        for (u8 i = 0; i < length; i++) {
                u8 loopI = (pos + i) % LOOP_SIZE;
                loop[loopI] = rev[i];
        }
}

void knotHash(char *key, u8 hash[]) {
        const int suffixLen = 5;
        const u8 suffix[] = {17, 31, 73, 47, 23};

        int keyLen = strlen(key);
        char augKey[keyLen + suffixLen + 1];
        strcpy(augKey, key);
        for (int i = 0; i < suffixLen; i++) {
                augKey[keyLen + i] = suffix[i];
        }
        augKey[keyLen + suffixLen] = '\0';

        u8 loop[LOOP_SIZE];
        for (int i = 0; i < LOOP_SIZE; i++) { loop[i] = i; }

        // Run hash 64 times to get sparse hash
        u8 pos = 0;
        int skip = 0;
        for (int i = 0; i < 64; i++) {
                for (u64 j = 0; j < strlen(augKey); j++) {
                        stepHash(loop, augKey[j], pos);
                        pos += (augKey[j] + skip) % LOOP_SIZE;
                        skip++;
                }
        }

        for (int i = 0; i < 16; i++) {
                int loopI = i * 16;
                hash[i] = loop[loopI];
                for (int j = 1; j < 16; j++) {
                        hash[i] ^= loop[loopI + j];
                }
        }
}

int getHammingWeight(u64 n) {
        int count = 0;
        while (n > 0) {
                n &= (n - 1);
                count++;
        }
        return count;
}

bool validPos(ivec2 pos) {
        return (pos.x >= 0) && (pos.y >= 0) && (pos.x < GRID_SIZE) && (pos.y < GRID_SIZE);
}

void bfs(int8 grid[][GRID_SIZE], int x, int y) {
        const ivec2 Dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}}; 

        int stackPtr = 1;
        ivec2 stack[100] = {0};
        stack[0] = (ivec2){{x, y}};


        while (stackPtr > 0) {
                ivec2 cur = stack[--stackPtr];

                if (grid[cur.y][cur.x] != -1) { continue; }
                grid[cur.y][cur.x] = 1;

                for (int i = 0; i < 4; i++) {
                        ivec2 next = ivec2Add(cur, Dirs[i]);
                        if (!validPos(next)) { continue; }
                        if (grid[next.y][next.x] == -1) {
                                stack[stackPtr++] = next;
                        }
                        if (stackPtr >= 100) {
                                fprintf(stderr, "Stack Overflow!!\n");
                        }
                }
        }
}

// 0: empty, -1: not-grouped, 1: grouped
int getNumGroups(int8 grid[][GRID_SIZE]) {
        int numGroups = 0;
        for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE; x++) {
                        if (grid[y][x] == -1) {
                                numGroups++;
                                bfs(grid, x, y);
                        }
                }
        }

        return numGroups;
}

void part1(struct input *input) {
        char *key = input->key;

        int hammingWeight = 0;
        for (int i = 0; i < GRID_SIZE; i++) {
                // Key long enough to hild base key plus row number and dash
                char rowkey[strlen(key) + 6];
                snprintf(rowkey, sizeof(rowkey), "%s-%d", key, i);

                u8 hash[16] = {0};
                knotHash(rowkey, hash);

                u64 msb = 0;
                u64 lsb = 0;
                for (int i = 0; i < 8; i++) {
                        msb <<= 8;
                        lsb <<= 8;
                        msb += hash[i];
                        lsb += hash[i+8];
                }
                hammingWeight += getHammingWeight(msb);
                hammingWeight += getHammingWeight(lsb);
                input->msb[i] = msb;
                input->lsb[i] = lsb;
        }

        printf("Part 1: Used Squares = %d\n\n", hammingWeight);
}

void part2(struct input *input) {
        u64 *hashMSB = input->msb;
        u64 *hashLSB = input->lsb;

        int8 grid[GRID_SIZE][GRID_SIZE] = {0};

        for (int y = 0; y < GRID_SIZE; y++) {
                for (int x = 0; x < GRID_SIZE / 2; x++) {
                        if ((hashMSB[y] >> (63 - x)) & 1) {
                                grid[y][x] = -1;
                        }
                        if ((hashLSB[y] >> (63 - x)) & 1) {
                                grid[y][x+(GRID_SIZE/2)] = -1;
                        }
                }
        }

        int numGroups = getNumGroups(grid);

        printf("Part 2: Groups = %d\n", numGroups);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;
        input.key = malloc(strlen(str) + 1);
        strcpy(input.key, str);

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day14.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
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
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
        //                 parseTime, pt1Time);

        return 0;
}

