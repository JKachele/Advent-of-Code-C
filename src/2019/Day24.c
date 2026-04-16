/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 15, 2026 20:38:56 UTC
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
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 16384

typedef struct tile {
        bool bug;
        int numAdj;
} tile;
typedef tal(tile**) taltile;

struct input {
        bool tile[5][5];
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printTile(bool tile[5][5]) {
        for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                        printf("%c", tile[y][x]? '#' : '.');
                }
                printf("\n");
        }
        printf("\n");
}

void printTileRecurse(taltile tiles) {
        for (int i = 0; i < (int)tiles.length; i++) {
                tile (*curTile)[5] = (tile(*)[5])tiles.array[i];
                for (int y = 0; y < 5; y++) {
                        for (int x = 0; x < 5; x++) {
                                // printf("%u", curTile[y][x].numAdj);
                                printf("%c", curTile[y][x].bug ? '#' : '.');
                        }
                        printf("\n");
                }
                printf("\n");
        }
}

void printBin(u32 n, int bits) {
        u32 mask = 1 << (bits - 1);
        for (int i = 0; i < bits; i++) {
                printf("%d", (n & mask) ? 1 : 0);
                mask >>= 1;
        }
        printf("\n");
}

u32 tileToId(bool tile[5][5]) {
        u32 id = 0;
        u32 mask = 1;
        for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                        if (tile[y][x])
                                id += mask;
                        mask <<= 1;
                }
        }
        return id;
}

bool validPos(int x, int y) {
        return x >= 0 && x < 5 && y >= 0 && y < 5;
}

// Also returns false of both x and y are 2 (Middle cell)
bool validPosRecursive(int x, int y) {
        bool xv = (x >= 0 && x < 5);
        bool yv = (y >= 0 && y < 5);
        bool inner = !(x == 2 && y == 2);
        return xv && yv && inner;
}

void stepTile(bool tile[5][5]) {
        const ivec2 dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        bool (*copy)[5] = (bool(*)[5])malloc(25 * sizeof(bool));
        memcpy(copy, tile, 25 * sizeof(bool));

        for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                        int numAdjacent = 0;
                        for (int d = 0; d < 4; d++) {
                                int xa = x + dirs[d].x;
                                int ya = y + dirs[d].y;
                                if (validPos(xa, ya) && copy[ya][xa])
                                        numAdjacent++;
                        }
                        if (copy[y][x] && numAdjacent != 1)
                                tile[y][x] = false;
                        if (!copy[y][x] && (numAdjacent == 1 || numAdjacent == 2))
                                tile[y][x] = true;
                }
        }
        free(copy);
}

int numOnSide(tile t[5][5], int side) {
        int numBugs = 0;
        for (int i = 0; i < 5; i++) {
                switch (side) {
                case 0: // Right
                        numBugs += t[i][4].bug ? 1 : 0;
                        break;
                case 1: // Bottom
                        numBugs += t[4][i].bug ? 1 : 0;
                        break;
                case 2: // Left
                        numBugs += t[i][0].bug ? 1 : 0;
                        break;
                case 3: // Top
                        numBugs += t[0][i].bug ? 1 : 0;
                        break;
                }
        }
        return numBugs;
}

void checkAdjacency(tile curTile[5][5], tile inner[5][5], tile outer[5][5]) {
        const ivec2 dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};

        for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                        if (x == 2 && y == 2) continue;
                        for (int d = 0; d < 4; d++) {
                                int xa = x + dirs[d].x;
                                int ya = y + dirs[d].y;
                                
                                // Inside current tile
                                if (validPosRecursive(xa, ya)) {
                                        curTile[y][x].numAdj += curTile[ya][xa].bug ? 1 : 0;
                                }

                                // Outside tile
                                if (outer != NULL) {
                                        if (xa == 5) // Outside Right
                                                curTile[y][x].numAdj += outer[2][3].bug ? 1 : 0;
                                        if (ya == 5) // Outside Down
                                                curTile[y][x].numAdj += outer[3][2].bug ? 1 : 0;
                                        if (xa == -1) // Outside Left
                                                curTile[y][x].numAdj += outer[2][1].bug ? 1 : 0;
                                        if (ya == -1) // Outside Up
                                                curTile[y][x].numAdj += outer[1][2].bug ? 1 : 0;
                                }

                                // Inside inner tile
                                if (inner != NULL && xa == 2 && ya == 2) {
                                        if (x == 3) // Inner Right
                                                curTile[y][x].numAdj += numOnSide(inner, 0);
                                        if (y == 3) // Inner Bottom
                                                curTile[y][x].numAdj += numOnSide(inner, 1);
                                        if (x == 1) // Inner Left
                                                curTile[y][x].numAdj += numOnSide(inner, 2);
                                        if (y == 1) // Inner Top
                                                curTile[y][x].numAdj += numOnSide(inner, 3);
                                }
                        }
                }
        }
}

void stepTileRecurse(taltile *tiles) {
        // Create new inner andouter Tile. Will remove at end if empty
        tile (*newOuter)[5] = calloc(25, sizeof(tile));
        tile (*newInner)[5] = calloc(25, sizeof(tile));
        tal_insert(*tiles, 0, (tile**)newOuter);
        tal_add(*tiles, (tile**)newInner);

        // Update adjacency of all tiles
        for (int i = 0; i < (int)tiles->length; i++) {
                tile (*curTile)[5] = (tile(*)[5])tiles->array[i];
                tile (*inner)[5] = NULL;
                tile (*outer)[5] = NULL;
                if (i < (int)tiles->length - 1)
                        inner = (tile(*)[5])tiles->array[i + 1];
                if (i != 0)
                        outer = (tile(*)[5])tiles->array[i - 1];
                checkAdjacency(curTile, inner, outer);
        }

        // Update bugs and reset adjacency count
        int innerBugs = 0;
        int outerBugs = 0;
        for (int i = 0; i < (int)tiles->length; i++) {
                tile (*cur)[5] = (tile(*)[5])tiles->array[i];
                for (int y = 0; y < 5; y++) {
                        for (int x = 0; x < 5; x++) {
                                bool bug = cur[y][x].bug;
                                int adj = cur[y][x].numAdj;
                                if (bug && adj != 1)
                                        cur[y][x].bug = false;
                                if (!bug && (adj == 1 || adj == 2))
                                        cur[y][x].bug = true;
                                cur[y][x].numAdj = 0;

                                if (i == 0 && cur[y][x].bug)
                                        outerBugs++;
                                if (i == (int)tiles->length - 1 && cur[y][x].bug)
                                        innerBugs++;
                        }
                }
        }

        // If outer or inner tile is empty, remove it
        if (outerBugs == 0)
                tal_remove(*tiles, 0);
        if (innerBugs == 0)
                tal_remove(*tiles, tiles->length - 1);
}

int countBugs(taltile tiles) {
        int numBugs = 0;
        for (int i = 0; i < (int)tiles.length; i++) {
                tile (*curTile)[5] = (tile(*)[5])tiles.array[i];
                for (int y = 0; y < 5; y++) {
                        for (int x = 0; x < 5; x++) {
                                if (curTile[y][x].bug)
                                        numBugs++;
                        }
                }
        }
        return numBugs;
}

void part1(struct input *input) {
        bool (*tile)[5] = (bool(*)[5])malloc(25 * sizeof(bool));
        memcpy(tile, input->tile, 25 * sizeof(bool));
        u32 id = tileToId(tile);

        bool *seen = calloc(0x2000000, sizeof(bool));
        seen[id] = true;

        for (;;) {
                stepTile(tile);
                id = tileToId(tile);
                if (seen[id])
                        break;
                seen[id] = true;
        }

        printf("Part 1: %u\n\n", id);
}

void part2(struct input *input) {
        const int NumMinutes = 200;

        taltile tiles = tal_init();
        tile (*initGrid)[5] = calloc(25, sizeof(tile));
        for (int y = 0; y < 5; y++)
                for (int x = 0; x < 5; x++)
                        initGrid[y][x].bug = input->tile[y][x];
        tal_add(tiles, (tile**)initGrid);

        for (int i = 0; i < NumMinutes; i++)
                stepTileRecurse(&tiles);
        // printTileRecurse(tiles);

        int numBugs = countBugs(tiles);

        printf("Part 2: %d\n", numBugs);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < 5; x++)
                        input.tile[y][x] = (str[x] == '#');
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day24.txt";
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

