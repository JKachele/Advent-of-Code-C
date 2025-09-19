/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 19, 2025 15:01:34 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <ctype.h>
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

enum type {
        SMALL,
        LARGE,
        START,
        END
};

typedef tll(u16) tllu16;

typedef struct cave {
        enum type type;
        tllu16 connect;
} cave;

struct input {
        u16 start;
        cave caves[0x100];
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void hashCollision(char *str, u8 hash, char *check[]) {
        if (check[hash] == NULL) {
                check[hash] = str;
                return;
        }
        if (strcmp(str, check[hash]) != 0) {
                printf("Collision at %u: %s and %s\n", hash, str, check[hash]);
                exit(1);
        }
}

bool isUpper(char c) {
        return c >= 'A' && c <= 'Z';
}

int32 countPaths(cave *caves, bool seen[], u8 curCave, bool part2) {
        if (caves[curCave].type == END)
                return 1;
        if (seen[curCave]) {
                if (caves[curCave].type == START)
                        return 0;
                if (caves[curCave].type == SMALL) {
                        // Can go in ONE small cave twice, Once that happens,
                        // Revert back to part 1 logic
                        if (part2)
                                part2 = false;
                        else
                                return 0;
                }
        }

        int32 paths = 0;
        tll_foreach(caves[curCave].connect, it) {
                bool newSeen[0x100];
                memcpy(newSeen, seen, 0x100);
                newSeen[curCave] = true;
                paths += countPaths(caves, newSeen, it->item, part2);
        }
        return paths;
}

void part1(struct input input) {
        cave *caves = input.caves;

        bool seen[0x100] = {0};
        int32 numPaths = countPaths(caves, seen, input.start, false);

        printf("Part 1: %d\n\n", numPaths);
}

void part2(struct input input) {
        cave *caves = input.caves;

        bool seen[0x100] = {0};
        int32 numPaths = countPaths(caves, seen, input.start, true);

        printf("Part 1: %d\n\n", numPaths);
}

struct input parseInput(llist *ll) {
        const u32 SEED = 0x12345678;
        struct input input = {0};
        cave *caves = input.caves;

        // Check for hash collisions
        char *check[0x100] = {0};

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;

                char *cave1 = strtok(str, "-");
                u8 hash1 = (u8)FNV(cave1, SEED);
                hashCollision(cave1, hash1, check);

                char *cave2 = strtok(NULL, "");
                u8 hash2 = (u8)FNV(cave2, SEED);
                hashCollision(cave2, hash2, check);

                cave *c1 = &caves[hash1];
                if (tll_length(c1->connect) == 0) {
                        if (strcmp(cave1, "start") == 0) {
                                c1->type = START;
                                input.start = hash1;
                        } else if (strcmp(cave1, "end") == 0) {
                                c1->type = END;
                        } else if (isUpper(cave1[0])) {
                                c1->type = LARGE;
                        } else {
                                c1->type = SMALL;
                        }
                }
                tll_push_back(c1->connect, hash2);

                cave *c2 = &caves[hash2];
                if (tll_length(c2->connect) == 0) {
                        if (strcmp(cave2, "start") == 0) {
                                c2->type = START;
                                input.start = hash2;
                        } else if (strcmp(cave2, "end") == 0) {
                                c2->type = END;
                        } else if (isUpper(cave2[0])) {
                                c2->type = LARGE;
                        } else {
                                c2->type = SMALL;
                        }
                }
                tll_push_back(c2->connect, hash1);

                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                // char *file = "assets/tests/2021/Day12.txt";
                // char *file = "assets/tests/2021/Day12b.txt";
                char *file = "assets/tests/2021/Day12c.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day12.txt";
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

