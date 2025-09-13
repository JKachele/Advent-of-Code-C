/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Aug 28, 2025 19:10:30 UTC
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
        EMPTY,          // '.'
        MIRROR_R,       // '/'
        MIRROR_L,       // '\'
        SPLIT_V,        // '|'
        SPLIT_H         // '-'
} tileType;

typedef struct {
        tileType type;
        bool energized;
        bool seen[2];   // 0: U/D, 1: L/R
} tile;

typedef enum {
        UP,
        RIGHT,
        DOWN,
        LEFT
} direction;

typedef struct {
        ivec2 pos;
        direction dir;
} light;

typedef struct {
        light a;
        light b;
} lightPair;

typedef tll(light) tlllight;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGrid(ivec2 size, tile grid[size.y][size.x]) {
        if (!Debug) return;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        switch (grid[y][x].type) {
                        case EMPTY:
                                printf(".");
                                break;
                        case MIRROR_R:
                                printf("/");
                                break;
                        case MIRROR_L:
                                printf("\\");
                                break;
                        case SPLIT_V:
                                printf("|");
                                break;
                        case SPLIT_H:
                                printf("-");
                                break;
                        }
                }
                printf("\n");
        }
        printf("\n");
}

void printEnergized(ivec2 size, tile grid[size.y][size.x]) {
        if (!Debug) return;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        printf("%c", grid[y][x].energized? '#' : '.');
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(ivec2 size, ivec2 pos) {
        return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

bool isEmpty(tile tile, direction dir) {
        if (tile.type == EMPTY)
                return true;
        if (tile.type == SPLIT_V && (dir == UP || dir == DOWN))
                return true;
        if (tile.type == SPLIT_H && (dir == LEFT || dir == RIGHT))
                return true;
        return false;
}

light reflect(ivec2 pos, direction dir, bool right) {
        light new = {0};

        // If a left mirror, flip the incoming direction 180
        if (!right)
                dir = (dir + 2) % 4;

        switch (dir) {
        case UP:
                new.dir = RIGHT;
                new.pos = (ivec2){{pos.x+1, pos.y}};
                break;
        case RIGHT:
                new.dir = UP;
                new.pos = (ivec2){{pos.x, pos.y-1}};
                break;
        case DOWN:
                new.dir = LEFT;
                new.pos = (ivec2){{pos.x-1, pos.y}};
                break;
        case LEFT:
                new.dir = DOWN;
                new.pos = (ivec2){{pos.x, pos.y+1}};
                break;
        }

        return new;
}

lightPair splitV(ivec2 pos) {
        light a;
        a.dir = UP;
        a.pos = (ivec2){{pos.x, pos.y-1}};

        light b;
        b.dir = DOWN;
        b.pos = (ivec2){{pos.x, pos.y+1}};

        return (lightPair){a, b};
}

lightPair splitH(ivec2 pos) {
        light a;
        a.dir = RIGHT;
        a.pos = (ivec2){{pos.x+1, pos.y}};

        light b;
        b.dir = LEFT;
        b.pos = (ivec2){{pos.x-1, pos.y}};

        return (lightPair){a, b};
}

void traceLight(ivec2 size, tile grid[][size.x], ivec2 initP, direction initD) {
        ivec2 dirs[] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
        tlllight queue = tll_init();

        light initLight = {initP, initD};
        tll_push_back(queue, initLight);

        while (tll_length(queue) > 0) {
                light cur = tll_pop_front(queue);

                // Continue in direction until leave map or run into somwthing
                ivec2 nextPos = cur.pos;
                tile *curTile;
                bool seen = false;
                while (validPos(size, nextPos) &&
                                isEmpty(grid[nextPos.y][nextPos.x], cur.dir)) {
                        curTile = &grid[nextPos.y][nextPos.x];
                        if (curTile->energized && curTile->seen[cur.dir%2]) {
                                seen = true;
                                break;
                        }
                        curTile->energized = true;
                        curTile->seen[cur.dir%2] = true;
                        nextPos = ivec2Add(nextPos, dirs[cur.dir]);
                }

                // If seen before in the same dir, move to next light
                if (seen)
                        continue;

                // If left the map, move to next light
                if (!validPos(size, nextPos))
                        continue;

                // If hit something
                curTile = &grid[nextPos.y][nextPos.x];
                curTile->energized = true;
                curTile->seen[cur.dir%2] = true;
                lightPair pair;
                switch (grid[nextPos.y][nextPos.x].type) {
                case MIRROR_R:
                        pair.a = reflect(nextPos, cur.dir, true);
                        tll_push_back(queue, pair.a);
                        break;
                case MIRROR_L:
                        pair.a = reflect(nextPos, cur.dir, false);
                        tll_push_back(queue, pair.a);
                        break;
                case SPLIT_V:
                        pair = splitV(nextPos);
                        tll_push_back(queue, pair.a);
                        tll_push_back(queue, pair.b);
                        break;
                case SPLIT_H:
                        pair = splitH(nextPos);
                        tll_push_back(queue, pair.a);
                        tll_push_back(queue, pair.b);
                        break;
                default:
                        printf("Stopped Too Early!\n");
                }
                // printEnergized(size, grid);
        }
}

int32 numEnergized(ivec2 size, tile grid[size.y][size.x]) {
        int32 count = 0;
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (grid[y][x].energized)
                                count++;
                }
        }
        return count;
}

void resetGrid(ivec2 size, tile grid[size.y][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        grid[y][x].energized = false;
                        grid[y][x].seen[0] = false;
                        grid[y][x].seen[1] = false;
                }
        }
}

void part1(llist *ll) {
        ivec2 size = {{getLongestLine(ll), ll->length}};
        tile grid[size.y][size.x];
        memset(grid, 0, size.y * size.x * sizeof(tile));

        llNode *current = ll->head;
        int32 yIndex = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size.x; x++) {
                        switch (str[x]) {
                        case '.':
                                grid[yIndex][x].type = EMPTY;
                                break;
                        case '/':
                                grid[yIndex][x].type = MIRROR_R;
                                break;
                        case '\\':
                                grid[yIndex][x].type = MIRROR_L;
                                break;
                        case '|':
                                grid[yIndex][x].type = SPLIT_V;
                                break;
                        case '-':
                                grid[yIndex][x].type = SPLIT_H;
                                break;
                        default:
                                printf("Unknown tile %c\n", str[x]);
                        }
                }

                current = current->next;
                yIndex++;
        }
        // printGrid(size, grid);

        traceLight(size, grid, (ivec2){0}, RIGHT);
        // printEnergized(size, grid);

        int32 energized = numEnergized(size, grid);

        printf("Part 1: %d\n\n", energized);
}

void part2(llist *ll) {
        ivec2 size = {{getLongestLine(ll), ll->length}};
        tile grid[size.y][size.x];
        memset(grid, 0, size.y * size.x * sizeof(tile));

        llNode *current = ll->head;
        int32 yIndex = 0;
        while(current != NULL) {
                char *str = (char*)current->data;

                for (int x=0; x<size.x; x++) {
                        switch (str[x]) {
                        case '.':
                                grid[yIndex][x].type = EMPTY;
                                break;
                        case '/':
                                grid[yIndex][x].type = MIRROR_R;
                                break;
                        case '\\':
                                grid[yIndex][x].type = MIRROR_L;
                                break;
                        case '|':
                                grid[yIndex][x].type = SPLIT_V;
                                break;
                        case '-':
                                grid[yIndex][x].type = SPLIT_H;
                                break;
                        default:
                                printf("Unknown tile %c\n", str[x]);
                        }
                }

                current = current->next;
                yIndex++;
        }
        printGrid(size, grid);

        int32 maxEnergized = 0;
        for (int x=0; x<size.x; x++) {
                ivec2 pos = {{x, 0}};
                traceLight(size, grid, pos, DOWN);
                int32 energized = numEnergized(size, grid);
                if (energized > maxEnergized)
                        maxEnergized = energized;
                resetGrid(size, grid);

                pos = (ivec2){{x, size.y-1}};
                traceLight(size, grid, pos, UP);
                energized = numEnergized(size, grid);
                if (energized > maxEnergized)
                        maxEnergized = energized;
                resetGrid(size, grid);
        }
        for (int y=0; y<size.y; y++) {
                ivec2 pos = {{0, y}};
                traceLight(size, grid, pos, RIGHT);
                int32 energized = numEnergized(size, grid);
                if (energized > maxEnergized)
                        maxEnergized = energized;
                resetGrid(size, grid);

                pos = (ivec2){{size.x-1, y}};
                traceLight(size, grid, pos, LEFT);
                energized = numEnergized(size, grid);
                if (energized > maxEnergized)
                        maxEnergized = energized;
                resetGrid(size, grid);
        }

        printf("Part 1: %d\n\n", maxEnergized);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day16.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day16.txt";
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

