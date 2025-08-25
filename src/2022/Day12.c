/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 17, 2025 16:42:52 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <limits.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

typedef struct {
        int height;
        int dist;
        bool seen;
} cell;

typedef tll(ivec2) ivectll;

static const ivec2 DIRS[4] = {{0,-1},{1,0},{0,1},{-1,0}};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGrid(const ivec2 SIZE, cell grid[SIZE.y][SIZE.x], bool dist) {
        if (!Debug) return;
        for (int y = 0; y < SIZE.y; y++) {
                for (int x = 0; x < SIZE.x; x++) {
                        int out = grid[y][x].height;
                        if (dist)
                                out = grid[y][x].dist;
                        if (out < 10) {
                                printf("%d  ", out);
                        } else {
                                printf("%d ", out);
                        }
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(const ivec2 SIZE, ivec2 pos) {
        if (pos.x < 0 || pos.y < 0)
                return false;
        if (pos.x >= SIZE.x || pos.y >= SIZE.y)
                return false;
        return true;
}

ivec2 lowestDist(const ivec2 SIZE, cell grid[SIZE.y][SIZE.x], ivectll *cells) {
        int minDist = INT_MAX;
        ivec2 minCell;

        tll_foreach(*cells, it) {
                ivec2 pos = it->item;
                int dist = grid[pos.y][pos.x].dist;
                if (dist < minDist) {
                        minDist = dist;
                        minCell = pos;
                }
        }

        tll_foreach(*cells, it) {
                if (ivec2Eq(it->item, minCell))
                        tll_remove(*cells, it);
        }

        return minCell;
}

void dijkstra(const ivec2 SIZE, cell grid[SIZE.y][SIZE.x], ivec2 start) {
        ivectll openCells = tll_init();
        tll_push_back(openCells, start);

        while (tll_length(openCells) > 0) {
                ivec2 cur = lowestDist(SIZE, grid, &openCells);

                int dist = grid[cur.y][cur.x].dist + 1;
                int height = grid[cur.y][cur.x].height;

                for (int i = 0; i < 4; i++) {
                        ivec2 new;
                        new.x = cur.x + DIRS[i].x;
                        new.y = cur.y + DIRS[i].y;
                        if (!validPos(SIZE, new)) continue;

                        cell *newCell = &grid[new.y][new.x];
                        if (newCell->height > height + 1) 
                                continue;
                        if (newCell->dist > dist)
                                newCell->dist = dist;
                        if (!newCell->seen) {
                                newCell->seen = true;
                                tll_push_back(openCells, new);
                        }
                }
        }
}

void dijkstraRev(const ivec2 SIZE, cell grid[SIZE.y][SIZE.x], ivec2 start) {
        ivectll openCells = tll_init();
        tll_push_back(openCells, start);

        while (tll_length(openCells) > 0) {
                ivec2 cur = lowestDist(SIZE, grid, &openCells);

                int dist = grid[cur.y][cur.x].dist + 1;
                int height = grid[cur.y][cur.x].height;

                for (int i = 0; i < 4; i++) {
                        ivec2 new;
                        new.x = cur.x + DIRS[i].x;
                        new.y = cur.y + DIRS[i].y;
                        if (!validPos(SIZE, new)) continue;

                        cell *newCell = &grid[new.y][new.x];
                        if (newCell->height < height - 1) 
                                continue;
                        if (newCell->dist > dist)
                                newCell->dist = dist;
                        if (!newCell->seen) {
                                newCell->seen = true;
                                tll_push_back(openCells, new);
                        }
                }
        }
}

void initCell(cell *c) {
        c->height = 0;
        c->dist = INT_MAX;
        c->seen = false;
}

void part1(llist *ll) {
        const ivec2 SIZE = {.x = getLongestLine(ll), .y = ll->length};
        cell grid[SIZE.y][SIZE.x];
        MAKE_LOOP(y, SIZE.y, x, SIZE.x)
                initCell(&grid[y][x]);

        ivec2 start = {.x = 0, .y = 0};
        ivec2 end = {.x = 0, .y = 0};

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                if ((int)strlen(str) != SIZE.x)
                        printf("INVALID INPUT SIZE ON LINE %d\n", line);

                for (int x = 0; x < SIZE.x; x++) {
                        if (str[x] == 'S') {
                                start = (ivec2){.x = x, .y = line};
                                grid[line][x].height = 0;
                                grid[line][x].dist = 0;
                                grid[line][x].seen = true;
                        } else if (str[x] == 'E') {
                                end = (ivec2){.x = x, .y = line};
                                grid[line][x].height = 25;
                        } else {
                                grid[line][x].height = str[x] - 'a';
                        }

                }

                current = current->next;
                line++;
        }
        printGrid(SIZE, grid, false);
        dijkstra(SIZE, grid, start);
        printGrid(SIZE, grid, true);

        int pathLen = grid[end.y][end.x].dist;

        printf("Part 1: Path Length: %d\n\n", pathLen);
}

void part2(llist *ll) {
        const ivec2 SIZE = {.x = getLongestLine(ll), .y = ll->length};
        cell grid[SIZE.y][SIZE.x];
        MAKE_LOOP(y, SIZE.y, x, SIZE.x)
                initCell(&grid[y][x]);

        ivectll starts = tll_init();
        ivec2 end = {.x = 0, .y = 0};

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                if ((int)strlen(str) != SIZE.x)
                        printf("INVALID INPUT SIZE ON LINE %d\n", line);

                for (int x = 0; x < SIZE.x; x++) {
                        if (str[x] == 'S' || str[x] == 'a') {
                                ivec2 start = {.x = x, .y = line};
                                tll_push_back(starts, start);
                                grid[line][x].height = 0;
                        } else if (str[x] == 'E') {
                                end = (ivec2){.x = x, .y = line};
                                grid[line][x].height = 25;
                                grid[line][x].dist = 0;
                                grid[line][x].seen = true;
                        } else {
                                grid[line][x].height = str[x] - 'a';
                        }

                }

                current = current->next;
                line++;
        }
        printGrid(SIZE, grid, false);
        dijkstraRev(SIZE, grid, end);
        printGrid(SIZE, grid, true);

        int minPathLen = INT_MAX;
        tll_foreach(starts, it) {
                ivec2 start = it->item;
                if (grid[start.y][start.x].dist < minPathLen)
                        minPathLen = grid[start.y][start.x].dist;
        }

        printf("Part 2: Shortest Path Length: %d\n\n", minPathLen);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day12.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day12.txt");
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

