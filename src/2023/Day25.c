/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Sep 11, 2025 17:59:16 UTC
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
#define SEED 0x12345678

typedef tll(u16) tllu16;
typedef tll(u32) tllu32;

struct input {
        tllu32 *parts;
        tllu32 partList;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printAdjMatrix(int32 numParts, u16 adjMatrix[numParts][numParts]) {
        if (!Debug) return;
        printf("   ");
        for (int i=0; i<numParts; i++) {
                printf("%02d ", i);
        }
        printf("\n");
        for (int i=0; i<numParts; i++) {
                printf("%02d: ", i);
                for (int j=0; j<numParts; j++) {
                        printf("%d  ", adjMatrix[i][j]);
                }
                printf("\n");
        }
        printf("\n");
}

// FNV Hashing Algorithm - H is seed
u32 FNV(const char *key, u32 h) {
        h ^= 2166136261UL;
        const u8 *data = (const u8*)key;
        for (int i=0; data[i]!='\0'; i++) {
                h ^= data[i];
                h *= 16777619;
        }
        return h;
}

void fillAdjMatrix(int numParts, u16 adjMatrix[numParts][numParts],
                   tllu32 *parts, tllu32 partList) {
        for (int i=0; i<numParts; i++) {
                for (int j=0; j<numParts; j++) {
                        adjMatrix[i][j] = 0;
                }
        }

        u16 *partIds = calloc(0x100000, sizeof(u16));
        int id = 0;
        tll_foreach(partList, it) {
                partIds[it->item] = id;
                id++;
        }

        tll_foreach(partList, it) {
                u16 part = partIds[it->item];
                tll_foreach(parts[it->item], it2) {
                        u16 part2 = partIds[it2->item];
                        adjMatrix[part][part2] = 1;
                        adjMatrix[part2][part] = 1;
                }
        }
}

void copyMatrix(int numParts, u16 src[][numParts], u16 dest[][numParts]) {
        for (int i=0; i<numParts; i++) {
                for (int j=0; j<numParts; j++) {
                        dest[i][j] = src[i][j];
                }
        }
}

// Finds a route between 2 points and backtracks deleting the edges along it
// Return ture if route found, false otherwise
bool deleteRoute(int numParts, u16 adjMatrix[numParts][numParts],
                u16 from, u16 to) {
        tllu16 queue = tll_init();
        tll_push_back(queue, from);

        bool seen[numParts];
        memset(seen, 0, numParts*sizeof(bool));
        int16 prevMap[numParts]; 
        for (int i=0; i<numParts; i++)
                prevMap[i] = -1;

        bool found = false;
        while (tll_length(queue) > 0) {
                u16 curPart = tll_pop_front(queue);

                for (int i=0; i<numParts; i++) {
                        if (adjMatrix[curPart][i] == 0)
                                continue;
                        if (seen[i])
                                continue;
                        seen[i] = true;
                        tll_push_back(queue, i);
                        prevMap[i] = curPart;
                        if (i == to) {
                                found = true;
                                break;
                        }
                }
                if (found)
                        break;
        }
        if (!found)
                return false;

        uint16 cur = to;
        debugP("    Route: %d ", cur);
        while (cur != from) {
                uint16 prev = prevMap[cur];
                debugP("%d ", prev);
                adjMatrix[cur][prev] = 0;
                adjMatrix[prev][cur] = 0;
                cur = prev;
        }
        debugP("\n");

        return true;
}

int numReachable(int numParts, u16 adjMatrix[numParts][numParts], u16 part) {
        tllu16 queue = tll_init();
        tll_push_back(queue, part);

        bool seen[numParts];
        memset(seen, 0, numParts*sizeof(bool));

        while (tll_length(queue) > 0) {
                u16 curPart = tll_pop_front(queue);

                for (int i=0; i<numParts; i++) {
                        if (adjMatrix[curPart][i] == 0)
                                continue;
                        if (seen[i])
                                continue;
                        seen[i] = true;
                        tll_push_back(queue, i);
                }
        }

        int numSeen = 0;
        for (int i=0; i<numParts; i++) {
                if (seen[i])
                        numSeen++;
        }
        return numSeen;
}

int splitGraph(int numParts, u16 adjMatrix[numParts][numParts]) {
        u16 start = 0;
        u16 end = 1;
        while (true) {
                debugP("Trying %d -> %d\n", start, end);
                int numElements = numParts * numParts;
                u16 (*matCpy)[numParts] = calloc(numElements, sizeof(u16));
                copyMatrix(numParts, adjMatrix, matCpy);
                for (int i=0; i<3; i++) {
                        debugP("  Itteration %d\n", i);
                        deleteRoute(numParts, matCpy, start, end);
                }

                int reachable = numReachable(numParts, matCpy, start);
                debugP("  Reachable: %d\n\n", reachable);

                // If graph is still connected
                if (reachable == numParts) {
                        end++;
                        continue;
                }

                return reachable * (numParts - reachable);
        }
}

void part1(struct input input) {
        tllu32 *parts = input.parts;
        tllu32 partList = input.partList;

        int numParts = tll_length(partList);
        u16 adjMatrix[numParts][numParts];
        fillAdjMatrix(numParts, adjMatrix, parts, partList);
        // printAdjMatrix(numParts, adjMatrix);

        int ans = splitGraph(numParts, adjMatrix);

        printf("Part 1: %d\n\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        const uint32 MASK = 0xFFFFF;

        input.partList = (tllu32)tll_init();
        input.parts = calloc(0x100000, sizeof(tllu16));
        if (input.parts == NULL)
                printf("ERROR: Failed to allocate memory for parts\n");

        char **names = calloc(0x100000, sizeof(char*));
        if (names == NULL)
                printf("ERROR: Failed to allocate memory for names\n");

        llNode *current = ll->head;
        int id = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                char *token = strtok(str, ": ");
                u32 connPart = FNV(token, SEED) & MASK;
                if (tll_length(input.parts[connPart]) == 0)
                        tll_push_back(input.partList, connPart);
                if (names[connPart] == NULL) {
                        names[connPart] = token;
                } else if (strcmp(names[connPart], token) != 0) {
                        printf("ERROR: Connection %s conflicts with %s\n", token, names[connPart]);
                }

                token = strtok(NULL, ": ");
                while(token!= NULL) {
                        u32 part = FNV(token, SEED) & MASK;
                        if (tll_length(input.parts[part]) == 0)
                                tll_push_back(input.partList, part);
                        if (names[part] == NULL) {
                                names[part] = token;
                        } else if (strcmp(names[part], token) != 0) {
                                printf("ERROR: Connection %s conflicts with %s\n", token, names[part]);
                        }
                        tll_push_back(input.parts[connPart], part);
                        tll_push_back(input.parts[part], connPart);
                        token = strtok(NULL, ": ");
                }

                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

