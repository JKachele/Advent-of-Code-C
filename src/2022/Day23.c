/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Aug 22, 2025 16:19:42 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef enum {
        NORTH,
        SOUTH,
        WEST,
        EAST
} dir;

typedef struct {
        int32 x1;
        int32 x2;
        int32 y1;
        int32 y2;
} ivec4;

typedef struct {
        ivec2 pos;
        ivec2 prevPos;
        bool canMove;
} elf;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printElvesPos(int32 numElves, elf *elves) {
        for (int i=0; i<numElves; i++) {
                elf e = elves[i];
                printf("(%d, %d)\n", e.pos.x, e.pos.y);
        }
}

void printElves(int32 numElves, elf *elves, ivec4 dim) {
        ivec2 size = {dim.x2 - dim.x1 + 1, dim.y2 - dim.y1 + 1};
        char grid[size.y][size.x];
        memset(grid, '.', sizeof(char) * (size.x * size.y));

        for (int i=0; i<numElves; i++) {
                ivec2 pos = elves[i].pos;
                grid[pos.y - dim.y1][pos.x - dim.x1] = '#';
        }

        for (int32 y=0; y<size.y; y++) {
                for (int32 x=0; x<size.x; x++) {
                        printf("%c", grid[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

int32 countElves(llist *ll) {
        int32 count = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;
                int len = strlen(str);
                for (int i=0; i<len; i++) {
                        if (str[i] == '#')
                                count++;
                }
                current = current->next;
        }

        return count;
}

int32 moveElves(int32 numElves, elf *elves) {
        int32 moved = 0;
        for (int32 i=0; i<numElves; i++) {
                ivec2 pos = elves[i].pos;
                if (!elves[i].canMove) {
                        elves[i].pos.x = elves[i].prevPos.x;
                        elves[i].pos.y = elves[i].prevPos.y;
                        continue;
                }

                for (int j=0; j<numElves; j++) {
                        if (i == j) continue;
                        ivec2 otherPos = elves[j].pos;
                        if (ivec2Eq(pos, otherPos)) {
                                elves[i].canMove = false;
                                elves[j].canMove = false;
                        }
                }

                if (!elves[i].canMove) {
                        elves[i].pos.x = elves[i].prevPos.x;
                        elves[i].pos.y = elves[i].prevPos.y;
                        continue;
                }
                elves[i].prevPos.x = elves[i].pos.x;
                elves[i].prevPos.y = elves[i].pos.y;
                elves[i].canMove = false;
                moved++;
        }
        return moved;
}

int32 stepElves(int32 numElves, elf *elves, int startingDir) {
        const ivec2 dirs[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        int32 numMoved = 0;
        for (int32 i=0; i<numElves; i++) {
                ivec2 pos = elves[i].pos;

                bool neighbors[4] = {false};
                int8 numNeighbors = 0;
                for (int j=0; j<numElves; j++) {
                        if (i == j) continue;
                        ivec2 otherPos = elves[j].prevPos;

                        if (abs(pos.x - otherPos.x) > 1) continue;
                        if (abs(pos.y - otherPos.y) > 1) continue;

                        numNeighbors++;
                        int dx = otherPos.x - pos.x;
                        int dy = otherPos.y - pos.y;
                        if (dy == -1) neighbors[NORTH] = true;
                        if (dy ==  1) neighbors[SOUTH] = true;
                        if (dx == -1) neighbors[WEST] = true;
                        if (dx ==  1) neighbors[EAST] = true;
                }

                if (numNeighbors == 0) continue; // Dont move if alone

                // Check in directions and move
                for (int j=0; j<4; j++) {
                        int8 dirIndex = (startingDir + j) % 4;
                        ivec2 dir = dirs[dirIndex];
                        
                        // Check for elves
                        bool elfPresent = neighbors[dirIndex];

                        if (!elfPresent) {
                                elves[i].pos.x += dir.x;
                                elves[i].pos.y += dir.y;
                                elves[i].canMove = true;
                                numMoved++;
                                break;
                        }
                }
        }
        if (numMoved > 0)
                numMoved = moveElves(numElves, elves);
        return numMoved;
}

ivec4 getDimensions(int32 numElves, elf *elves) {
        ivec4 dimensions = {0};
        for (int32 i=0; i<numElves; i++) {
                ivec2 pos = elves[i].pos;
                if (pos.x < dimensions.x1) dimensions.x1 = pos.x;
                if (pos.x > dimensions.x2) dimensions.x2 = pos.x;
                if (pos.y < dimensions.y1) dimensions.y1 = pos.y;
                if (pos.y > dimensions.y2) dimensions.y2 = pos.y;
        }
        return dimensions;
}

void part1(llist *ll) {
        int32 numElves = countElves(ll);
        elf elves[numElves];
        memset(elves, 0, sizeof(elf)*numElves);

        int32 indexy = 0;
        int32 elfIndex = 0;
        llNode *current = ll->head;
        while(current != NULL && elfIndex < numElves) {
                char *str = (char*)current->data;
                int len = strlen(str);
                for (int i=0; i<len; i++) {
                        if (str[i] == '#') {
                                elves[elfIndex].pos.x = i;
                                elves[elfIndex].prevPos.x = i;
                                elves[elfIndex].pos.y = indexy;
                                elves[elfIndex].prevPos.y = indexy;
                                elves[elfIndex].canMove = false;
                                elfIndex++;
                        }
                        if (elfIndex >= numElves)
                                break;
                }
                current = current->next;
                indexy++;
        }
        // printElves(numElves, elves, (ivec4){-3, 10, -2, 9});


        for (int i=0; i< 10; i++) {
                stepElves(numElves, elves, i % 4);
                // printElves(numElves, elves, (ivec4){-3, 10, -2, 9});
        }
        ivec4 dim = getDimensions(numElves, elves);
        int32 area = (dim.x2 - dim.x1 + 1) * (dim.y2 - dim.y1 + 1);
        int32 emptyTiles = area - numElves;

        printf("Part 1: Empty tiles: %d\n\n", emptyTiles);
}

void part2(llist *ll) {
        int32 numElves = countElves(ll);
        elf elves[numElves];
        memset(elves, 0, sizeof(elf)*numElves);

        int32 indexy = 0;
        int32 elfIndex = 0;
        llNode *current = ll->head;
        while(current != NULL && elfIndex < numElves) {
                char *str = (char*)current->data;
                int len = strlen(str);
                for (int i=0; i<len; i++) {
                        if (str[i] == '#') {
                                elves[elfIndex].pos.x = i;
                                elves[elfIndex].prevPos.x = i;
                                elves[elfIndex].pos.y = indexy;
                                elves[elfIndex].prevPos.y = indexy;
                                elves[elfIndex].canMove = false;
                                elfIndex++;
                        }
                        if (elfIndex >= numElves)
                                break;
                }
                current = current->next;
                indexy++;
        }
        // printElves(numElves, elves, (ivec4){-3, 10, -2, 9});

        int32 round = 0;
        while(true) {
                int32 moved = stepElves(numElves, elves, round % 4);
                round++;
                if (moved == 0) break;
        }

        printf("Part 2: Round with no moves: %d\n", round);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2022/Day23.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2022/Day23.txt";
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

