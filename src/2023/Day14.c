/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Aug 27, 2025 19:30:15 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
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
#define ROLLS 1000000000
#define SEED 0x12345678

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

// FNV Hashing Algorithm - H is seed
uint32 FNV(const char *key, uint32 h) {
        h ^= 2166136261UL;
        const uint8 *data = (const uint8*)key;
        for (int i=0; data[i]!='\0'; i++) {
                h ^= data[i];
                h *= 16777619;
        }
        return h;
}

void printPlatform(ivec2 size, char platform[size.y][size.x]) {
        if (!Debug) return;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        printf("%c", platform[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

// dir: 0=UP, 1=LEFT, 2=DOWN, 3=RIGHT
void tiltPLatform(ivec2 size, char platform[size.y][size.x], int8 dir) {
        bool vert = (dir == 0 || dir == 2);
        bool invert = (dir == 2 || dir == 3);

        int sizeI = vert ? size.x : size.y;
        int sizeJ = vert ? size.y : size.x;

        for (int i=0; i<sizeI; i++) {
                int openSpace = invert ? sizeJ - 1 : 0;
                for (int j=0; j<sizeJ; j++) {
                        int jInv = invert ? (sizeJ - 1) - j : j;
                        int x = vert ? i : jInv;
                        int y = vert ? jInv : i;

                        if (platform[y][x] == '#')
                                openSpace = invert ? jInv - 1 : jInv + 1;
                        if (platform[y][x] == 'O') {
                                platform[y][x] = '.';
                                if (vert) 
                                        platform[openSpace][x] = 'O';
                                else
                                        platform[y][openSpace] = 'O';
                                if (invert)
                                        openSpace--;
                                else
                                        openSpace++;
                        }
                }
        }
}

void rollPlatform(ivec2 size, char platform[size.y][size.x]) {
        for (int i=0; i<4; i++) {
                tiltPLatform(size, platform, i);
        }
}

void spinCycle(ivec2 size, char platform[size.y][size.x]) {
        char p[size.y+1][size.x];
        memcpy(p, platform, (size.y+1) * size.x);

        int32 *stateMap = calloc(UINT16_MAX+1, sizeof(int32));
        // Only skip ahead after 2 consecutive matches (Avoid false positives)
        bool found = false;
        bool finish = false;
        for (int i=1; i<=ROLLS; i++) {
                rollPlatform(size, platform);
                if (finish) continue;

                uint16 hash = (uint16)FNV((char*)platform, SEED);
                if (stateMap[hash] == 0) {
                        stateMap[hash] = i;
                        if (found) found = false;
                        continue;
                }

                // If match found
                if (!found) {
                        found = true;
                        continue;
                }

                // If 2 consecutive matches
                int32 cycle = i - stateMap[hash];
                int32 rollsLeft = ROLLS - i;
                int32 cyclesLeft = rollsLeft / cycle;
                i = i + (cycle * cyclesLeft);
                finish = true;
        }
}

int64 calcWeight(ivec2 size, char platform[size.y][size.x]) {
        int64 weight = 0;
        for (int i=1; i<=size.y; i++) {
                int y = size.y - i;
                for (int x=0; x<size.x; x++) {
                        if (platform[y][x] == 'O')
                                weight += i;
                }
        }
        return weight;
}

void part1(llist *ll) {
        ivec2 size = {{getLongestLine(ll), ll->length}};
        char platform[size.y][size.x];

        llNode *current = ll->head;
        int32 yind = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                strncpy(platform[yind], str, size.x);

                current = current->next;
                yind++;
        }
        // printPlatform(size, platform);
        tiltPLatform(size, platform, 0);
        // printPlatform(size, platform);

        int64 weight = calcWeight(size, platform);

        printf("Part 1: %ld\n\n", weight);
}

void part2(llist *ll) {
        ivec2 size = {{getLongestLine(ll), ll->length}};
        char platform[size.y+1][size.x]; // +1 to add null terminator for FNV
        platform[size.y][0] = '\0'; // null terminator for FNV

        llNode *current = ll->head;
        int32 yind = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                strncpy(platform[yind], str, size.x);

                current = current->next;
                yind++;
        }
        // printPlatform(size, platform);

        spinCycle(size, platform);

        int64 weight = calcWeight(size, platform);

        printf("Part 2: %ld\n\n", weight);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day14.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
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

