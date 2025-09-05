/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 05, 2025 15:49:00 UTC
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

typedef union brickCoord {
        int32 raw[6];
        struct {
                ivec3 a;
                ivec3 b;
                int32 index;
        };
} brickCoord;

typedef tll(brickCoord) tllbrickCoord;

typedef tll(ivec3) tllivec3;
typedef tll(int32) tllint32;

typedef struct brick {
        int32 id;
        tllivec3 cubes;
        tllint32 supporting;
        bool onGround;
} brick;

typedef tll(brick) tllbrick;

struct input {
        tllbrickCoord brickCoords;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printBricks(tllbrickCoord bricks) {
        if (!Debug) return;
        tll_foreach(bricks, it) {
                brickCoord b = it->item;
                printf("%d: {%d, %d, %d} -> ", b.index, b.a.x, b.a.y, b.a.z);
                printf("{%d, %d, %d}\n", b.b.x, b.b.y, b.b.z);
        }
        printf("\n");
}

void printStack(ivec3 size, int32 stack[][size.y][size.x]) {
        if (!Debug) return;
        int zStart = size.z < 21 ? size.z-1 : 20;

        printf("X:\n");
        for (int z=zStart; z>=0; z--) {
                for (int x=0; x<size.x; x++) {
                        int id = -1;
                        for (int y=0; y<size.y; y++) {
                                if (stack[z][y][x] != -1)
                                        id = stack[z][y][x];
                        }
                        if (id == -1)
                                printf("  .  ");
                        else if (id == -2)
                                printf("-----");
                        else
                                // printf("%c ", id + 'A');
                                printf("%04d ", id);
                }
                printf("\n");
        }
        printf("\n");

        printf("y:\n");
        for (int z=zStart; z>=0; z--) {
                for (int y=0; y<size.y; y++) {
                        int id = -1;
                        for (int x=0; x<size.x; x++) {
                                if (stack[z][y][x] != -1)
                                        id = stack[z][y][x];
                        }
                        if (id == -1)
                                printf("  .  ");
                        else if (id == -2)
                                printf("-----");
                        else
                                // printf("%c ", id + 'A');
                                printf("%04d ", id);
                }
                printf("\n");
        }
        printf("\n");
}

ivec3 getSize(tllbrickCoord bricks) {
        ivec3 size = {{INT32_MIN, INT32_MIN, INT32_MIN}};
        tll_foreach(bricks, it) {
                brickCoord brick = it->item;
                for (int i = 0; i < 6; i++) {
                        if (brick.raw[i] > size.raw[i%3])
                                size.raw[i%3] = brick.raw[i];
                }
        }
        return ivec3Add(size, (ivec3){{1, 1, 1}});
}

tllbrick expandBrick(tllbrickCoord brickCoords) {
        tllbrick bricks = tll_init();
        tll_foreach(brickCoords, it) {
                brickCoord bc = it->item;
                brick b = {0};
                b.id = bc.index;

                for (int z = bc.a.z; z <= bc.b.z; z++) {
                        for (int y = bc.a.y; y <= bc.b.y; y++) {
                                for (int x = bc.a.x; x <= bc.b.x; x++) {
                                        ivec3 cube = {{x, y, z}};
                                        tll_push_back(b.cubes, cube);
                                }
                        }
                }
                tll_push_back(bricks, b);
        }
        return bricks;
}

void populateStack(ivec3 size, int32 stack[][size.y][size.x], tllbrick bricks) {
        MAKE_LOOP(z, size.z, y, size.y, x, size.x) {
                if (z == 0)
                        stack[z][y][x] = -2;
                else
                        stack[z][y][x] = -1;
        }
        tll_foreach(bricks, it) {
                brick b = it->item;
                tll_foreach(b.cubes, itt) {
                        ivec3 cube = itt->item;
                        stack[cube.z][cube.y][cube.x] = b.id;
                }
        }
}

void dropBrick(ivec3 size, int32 stack[][size.y][size.x], brick brick) {
        tll_foreach(brick.cubes, it) {
                ivec3 *pos = &it->item;
                stack[pos->z][pos->y][pos->x] = -1;
                pos->z--;
                stack[pos->z][pos->y][pos->x] = brick.id;
        }
}

bool fallStack(ivec3 size, int32 stack[][size.y][size.x], tllbrick bricks) {
        bool dropped = false;
        tll_foreach(bricks, it) {
                brick b = it->item;
                bool canDrop = true;
                tll_foreach(b.cubes, itt) {
                        ivec3 pos = itt->item;
                        int32 under = stack[pos.z-1][pos.y][pos.x];
                        if (under != -1 && under != b.id)
                                canDrop = false;
                }
                if (canDrop) {
                        dropBrick(size, stack, b);
                        dropped = true;
                }
        }
        return dropped;
}

int32 numSupported(ivec3 size, int32 stack[][size.y][size.x], tllbrick bricks) {
        int32 numBricks = tll_length(bricks);
        bool good[numBricks];
        memset(good, 1, numBricks * sizeof(bool));
        
        tll_foreach(bricks, it) {
                brick *b = &it->item;
                bool support[numBricks];
                memset(support, 0, numBricks * sizeof(bool));
                tll_foreach(b->cubes, itt) {
                        ivec3 pos = itt->item;
                        int32 under = stack[pos.z-1][pos.y][pos.x];
                        if (under == -2) {
                                b->onGround = true;
                        } else if (under != -1 && under != b->id) {
                                if (!support[under]) {
                                        tll_push_back(b->supporting, under);
                                        support[under] = true;
                                }
                        }
                }
                if (tll_length(b->supporting) == 1) {
                        good[tll_front(b->supporting)] = false;
                }
        }

        int32 numSupported = 0;
        for (int i=0; i<numBricks; i++) {
                if (good[i]) {
                        numSupported++;
                        debugP("%c\n", i + 'A');
                }
        }
        debugP("\n");
        return numSupported;
}

int32 chainReact(tllbrick bricks, int32 removeId) {
        int32 numBricks = tll_length(bricks);
        bool intact[numBricks];
        memset(intact, 1, numBricks * sizeof(bool));
        intact[removeId] = false;

        bool bricksBroken;
        do {
                bricksBroken = false;
                tll_foreach(bricks, it) {
                        brick b = it->item;
                        if (!intact[b.id])
                                continue;
                        // Cannot be broken if on ground
                        if (b.onGround)
                                continue;
                        bool broken = true;
                        tll_foreach(b.supporting, itt) {
                                if (intact[itt->item]) {
                                        broken = false;
                                        break;
                                }
                        }
                        if (broken) {
                                intact[b.id] = false;
                                bricksBroken = true;
                        }
                }
        } while (bricksBroken);

        int32 numBroken = -1; // -1 because don't count original removed brick
        for (int i=0; i<numBricks; i++) {
                if (!intact[i])
                        numBroken++;
        }
        return numBroken;
}

void part1(struct input input) {
        ivec3 size = getSize(input.brickCoords);
        tllbrick bricks = expandBrick(input.brickCoords);

        int32 stack[size.z][size.y][size.x];
        populateStack(size, stack, bricks);
        // printStack(size, stack);

        bool dropped = true;
        while (dropped == true) {
                dropped = fallStack(size, stack, bricks);
        }
        // printStack(size, stack);

        int32 supported = numSupported(size, stack, bricks);

        printf("Part 1: %d\n\n", supported);
}

void part2(struct input input) {
        ivec3 size = getSize(input.brickCoords);
        tllbrick bricks = expandBrick(input.brickCoords);

        int32 stack[size.z][size.y][size.z];
        populateStack(size, stack, bricks);
        // printStack(size, stack);

        bool dropped = true;
        while (dropped == true) {
                dropped = fallStack(size, stack, bricks);
        }
        printStack(size, stack);

        // Populate the list of supporting bricks for each brick
        numSupported(size, stack, bricks);

        int64 totalBroken = 0;
        tll_foreach(bricks, it) {
                brick b = it->item;
                int32 numBroken = chainReact(bricks, b.id);
                debugP("%d: %d\n", b.id, numBroken);
                totalBroken += numBroken;
        }

        printf("Part 2: %ld\n", totalBroken);
}

struct input parseInput(llist *ll) {
        struct input input;
        input.brickCoords = (tllbrickCoord)tll_init();

        llNode *current = ll->head;
        int i = 0;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                brickCoord curBrick;
                char *token = strtok(str, ",~");
                for (int i=0; i<6; i++) {
                        curBrick.raw[i] = strtol(token, (char**)NULL, 10);
                        token = strtok(NULL, ",~");
                }
                curBrick.index = i;

                tll_push_back(input.brickCoords, curBrick);
                current = current->next;
                i++;
        }
        printBricks(input.brickCoords);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day22.txt";
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

