/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Aug 27, 2025 19:30:15 UTC
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

typedef enum {
        EMPTY,
        SQUARE,
        ROUND
} rock;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPlatform(ivec2 size, rock platform[size.y][size.x]) {
        if (!Debug) return;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (platform[y][x] == EMPTY)
                                printf(".");
                        else if (platform[y][x] == SQUARE)
                                printf("#");
                        else if (platform[y][x] == ROUND)
                                printf("O");
                }
                printf("\n");
        }
        printf("\n");
}

void rollPLatform(ivec2 size, rock platform[size.y][size.x]) {
        for (int x=0; x<size.x; x++) {
                int openSpace = 0;
                for (int y=0; y<size.y; y++) {
                        if (platform[y][x] == SQUARE)
                                openSpace = y + 1;
                        if (platform[y][x] == ROUND) {
                                platform[y][x] = EMPTY;
                                platform[openSpace][x] = ROUND;
                                openSpace++;
                        }
                }
        }
}

int64 calcWeight(ivec2 size, rock platform[size.y][size.x]) {
        int64 weight = 0;
        for (int i=1; i<=size.y; i++) {
                int y = size.y - i;
                for (int x=0; x<size.x; x++) {
                        if (platform[y][x] == ROUND)
                                weight += i;
                }
        }
        return weight;
}

void part1(llist *ll) {
        ivec2 size = {getLongestLine(ll), ll->length};
        rock platform[size.y][size.x];

        llNode *current = ll->head;
        int32 yind = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size.x; x++) {
                        if (str[x] == '.')
                                platform[yind][x] = EMPTY;
                        else if (str[x] == '#')
                                platform[yind][x] = SQUARE;
                        else if (str[x] == 'O')
                                platform[yind][x] = ROUND;
                        else
                                printf("%c at (%d, %d)\n", str[x], x, yind);
                }       

                current = current->next;
                yind++;
        }
        // printPlatform(size, platform);
        rollPLatform(size, platform);
        // printPlatform(size, platform);
        int64 weight = calcWeight(size, platform);

        printf("Part 1: %ld\n\n", weight);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
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

