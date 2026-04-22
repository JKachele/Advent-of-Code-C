/*************************************************
 *File----------Day03.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 22, 2026 13:14:36 UTC
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
#include "../lib/tllist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 16384

typedef struct claim {
        ivec2 pos;
        ivec2 size;
} claim;
typedef tll(claim) tllclaim;

struct input {
        tllclaim claims;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void part1(struct input *input) {
        ivec2 size = {0};
        tll_foreach(input->claims, it) {
                claim c = it->item;
                if (c.pos.x + c.size.x > size.x)
                        size.x = c.pos.x + c.size.x;
                if (c.pos.y + c.size.y > size.y)
                        size.y = c.pos.y + c.size.y;
        }
        size.x++;
        size.y++;

        u8 (*fabric)[size.x] = (u8(*)[size.x])calloc(size.x * size.y, sizeof(u8));
        tll_foreach(input->claims, it) {
                claim c = it->item;
                for (int y = c.pos.y; y < c.pos.y + c.size.y; y++) {
                        for (int x = c.pos.x; x < c.pos.x + c.size.x; x++) {
                                fabric[y][x]++;
                        }
                }
        }

        int numOverlaps = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (fabric[y][x] > 1)
                                numOverlaps++;
                }
        }

        printf("Part 1: %d\n\n", numOverlaps);
}

void part2(struct input *input) {
        ivec2 size = {0};
        tll_foreach(input->claims, it) {
                claim c = it->item;
                if (c.pos.x + c.size.x > size.x)
                        size.x = c.pos.x + c.size.x;
                if (c.pos.y + c.size.y > size.y)
                        size.y = c.pos.y + c.size.y;
        }
        size.x++;
        size.y++;

        u8 (*fabric)[size.x] = (u8(*)[size.x])calloc(size.x * size.y, sizeof(u8));
        tll_foreach(input->claims, it) {
                claim c = it->item;
                for (int y = c.pos.y; y < c.pos.y + c.size.y; y++) {
                        for (int x = c.pos.x; x < c.pos.x + c.size.x; x++) {
                                fabric[y][x]++;
                        }
                }
        }

        int intactClaim = 1;
        tll_foreach(input->claims, it) {
                claim c = it->item;
                bool intact = true;
                for (int y = c.pos.y; y < c.pos.y + c.size.y; y++) {
                        for (int x = c.pos.x; x < c.pos.x + c.size.x; x++) {
                                if (fabric[y][x] > 1) {
                                        intact = false;
                                        break;
                                }
                        }
                        if (!intact) break;
                }
                if (intact) break;
                intactClaim++;
        }

        printf("Part 2: %d\n", intactClaim);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                claim cur = {0};

                char *tok = strtok(str, "@");
                cur.pos.x = strtol(strtok(NULL, ","), NULL, 10);
                cur.pos.y = strtol(strtok(NULL, ":"), NULL, 10);
                cur.size.x = strtol(strtok(NULL, "x"), NULL, 10);
                cur.size.y = strtol(strtok(NULL, ""), NULL, 10);
                tll_push_back(input.claims, cur);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day03.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day03.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day03.txt";
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

        return 0;
}

