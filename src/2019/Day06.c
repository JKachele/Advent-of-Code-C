/*************************************************
 *File----------Day06.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 30, 2026 16:12:15 UTC
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
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(u32) tllu32;

typedef struct orbit {
        u32 center;
        u32 orbiter;
} orbit;
typedef tll(orbit) tllorbit;

typedef struct object {
        u32 center;
        tllu32 orbits;
} object;

struct input {
        tllorbit orbits;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void idToName(u32 id, char str[4]) {
        const u32 Mask = 0x3F;
        for (int i = 2; i >= 0; i--) {
                u32 num = id & Mask;
                if (num < 10)
                        str[i] = num + '0';
                else
                        str[i] = (num - 10) + 'A';
                id >>= 6;
        }
        str[3] = '\0';
}

void printOrbits(tllorbit orbits) {
        tll_foreach(orbits, it) {
                orbit o = it->item;
                char center[4];
                idToName(o.center, center);
                char orbiter[4];
                idToName(o.orbiter, orbiter);
                printf("Center: %s, Orbiter: %s\n", center, orbiter);
        }
        printf("\n");
}

u32 nameToId(char str[3]) {
        u32 id = 0;
        // Names can have A-Z and 0-9. 6 bits per char - 18-bit id
        for (int i = 0; i < 3; i++) {
                id <<= 6;
                if (str[i] <= '9')
                        id += str[i] - '0';
                else
                        id += (str[i] - 'A') + 10;
        }
        return id;
}

int numOrbits(tllu32 *orbitTree, u32 center, int depth) {
        int num = depth;
        tll_foreach(orbitTree[center], it) {
                num += numOrbits(orbitTree, it->item, depth + 1);
        }
        return num;
}

int numTransfers(object *orbitTree, u32 current, u32 prev) {
        const u32 SAN = nameToId("SAN");
        
        if (orbitTree[current].center != prev) {
                int jumps = numTransfers(orbitTree, orbitTree[current].center, current);
                if (jumps != -1)
                        return jumps + 1;
        }

        tll_foreach(orbitTree[current].orbits, it) {
                if (it->item == SAN)
                        return 0;
                if (it->item == prev)
                        continue;

                int jumps = numTransfers(orbitTree, it->item, current);
                if (jumps != -1)
                        return jumps + 1;
        }

        return -1;
}

void part1(struct input *input) {
        const u32 COM = nameToId("COM");
        // printOrbits(input->orbits);

        // Map of centers to their direct orbiters
        tllu32 *orbitTree = calloc(0x40000, sizeof(tllu32));
        tll_foreach(input->orbits, it) {
                orbit o = it->item;
                tll_push_back(orbitTree[o.center], o.orbiter);
        }

        int totalOrbits = numOrbits(orbitTree, COM, 0);

        printf("Part 1: %d\n\n", totalOrbits);
}

void part2(struct input *input) {
        const u32 YOU = nameToId("YOU");

        object *orbitTree = calloc(0x40000, sizeof(object));
        tll_foreach(input->orbits, it) {
                orbit o = it->item;
                orbitTree[o.orbiter].center = o.center;
                tll_push_back(orbitTree[o.center].orbits, o.orbiter);
        }

        int transfers = numTransfers(orbitTree, orbitTree[YOU].center, YOU);

        printf("Part 2: %d\n", transfers);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                orbit o = {0};
                o.center = nameToId(str);
                o.orbiter = nameToId(str+4);
                tll_push_back(input.orbits, o);
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
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

