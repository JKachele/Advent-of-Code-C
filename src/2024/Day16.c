/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Jan 14, 2025 16:41:13 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../lib/tllist.h"

#define STRAIGHT 1
#define TURN 101

typedef struct {
        int sizeX;
        int sizeY;
} data;

typedef struct {
        char type;
        bool onPath;
        bool seen[4];
        long dist[4];
} cell;

typedef struct {
        int x;
        int y;
        direction dir;
} entry;

typedef tll(ivec2) ivec2ll;
typedef tll(entry) entryTll;

bool entryEq(entry a, entry b) {
        return (a.x == b.x && a.y == b.y && a.dir == b.dir);
}

void printGrid(data d, cell grid[d.sizeY][d.sizeX]) {
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        printf("%c", grid[y][x].type);
                }
                printf("\n");
        }
        printf("\n");
}

void printPath(data d, cell grid[d.sizeY][d.sizeX]) {
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        if (grid[y][x].type == '#')
                                printf("#");
                        else if (grid[y][x].onPath)
                                printf("O");
                        else
                                printf(".");
                }
                printf("\n");
        }
        printf("\n");
}

void printOpenCells(ivec2ll cells) {
        tll_foreach(cells, it) {
                ivec2 v = it->item;
                printf("[%d, %d] ", v.x, v.y);
        }
        printf("\n");
}

entry lowestDist(data d, cell grid[d.sizeY][d.sizeX], entryTll *cells) {
        long minDist = LONG_MAX;
        entry minCell;

        tll_foreach(*cells, it) {
                entry pos = it->item;
                long dist = grid[pos.y][pos.x].dist[pos.dir];
                if (dist < minDist) {
                        minDist = dist;
                        minCell = pos;
                }
        }

        tll_foreach(*cells, it) {
                if (entryEq(it->item, minCell))
                        tll_remove(*cells, it);
        }
        return minCell;
}

int isOpenCell(data d, cell grid[d.sizeY][d.sizeX], ivec2 cell) {
        if (cell.x < 0 || cell.y < 0 || cell.x >= d.sizeX || cell.y >= d.sizeY)
                return 0;
        if (grid[cell.y][cell.x].type == '#')
                return 0;
        return 1;
}

long dijkstra(data d, cell grid[d.sizeY][d.sizeX],
                entry start, entry end, bool backtrack) {
        ivec2 dirs[4] = {{0,-1},{1,0},{0,1},{-1,0}};

        entryTll openCells = tll_init();

        if (backtrack) {
                for (direction i = NORTH; i < 4; i++) {
                        entry e = {start.x, start.y, i};
                        tll_push_back(openCells, e);
                }
        } else {
                tll_push_back(openCells, start);
        }

        // Main Dijkstra loop
        // Notes: cell.seen: 0:Not Seen, 1-Seen, 2-seen
        while (tll_length(openCells) > 0) {
                // printf("Num Open cells: %zu\n", tll_length(openCells));
                
                // Find cell with lowest distance
                entry curEntry = lowestDist(d, grid, &openCells);
                cell *cur = &grid[curEntry.y][curEntry.x];
                direction dir = curEntry.dir;
                long dist = cur->dist[dir];

                // Set turn distances
                for (int i = 1; i < 4; i++) {
                        direction newDir = (dir + i) % 4;
                        if (cur->dist[newDir] > dist + 1000)
                                cur->dist[newDir] = dist + 1000;
                        if (!cur->seen[newDir]) {
                                entry new = {curEntry.x, curEntry.y, newDir};
                                tll_push_back(openCells, new);
                                cur->seen[newDir] = true;
                        }
                }

                // Set next cell
                ivec2 nextPos = {curEntry.x + dirs[dir].x,
                                 curEntry.y + dirs[dir].y};
                if (isOpenCell(d, grid, nextPos)) {
                        cell *next = &grid[nextPos.y][nextPos.x];
                        if (next->dist[dir] > dist + 1) {
                                next->dist[dir] = dist + 1;
                                if (!next->seen[dir]) {
                                        entry new = {nextPos.x, nextPos.y, dir};
                                        tll_push_back(openCells, new);
                                }
                        }

                }
        }

        long endDist = LONG_MAX;
        for (int i = 0; i < 4; i++) {
                if (grid[end.y][end.x].dist[i] < endDist)
                        endDist = grid[end.y][end.x].dist[i];
        }
        return endDist;
}

int backTrace(data d, cell startGrid[d.sizeY][d.sizeX],
                cell endGrid[d.sizeY][d.sizeX], long minDist) {

        direction flip[] = {SOUTH, WEST, NORTH, EAST};

        int onPath = 0;
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        for (direction c = NORTH; c < 4; c++) {
                                long startDist = startGrid[y][x].dist[c];
                                long endDist = endGrid[y][x].dist[flip[c]];
                                if (startDist + endDist == minDist) {
                                        startGrid[y][x].onPath = true;
                                        endGrid[y][x].onPath = true;
                                        onPath++;
                                        break;
                                }
                        }
                }
        }

        return onPath;
}

void initCell(cell *c) {
        c->type = 0;
        c->onPath = false;
        for (int i = 0; i < 4; i++) {
                c->seen[i] = false;
                c->dist[i] = LONG_MAX;
        }
}

void part1_2(llist *ll) {
        data d;
        d.sizeY = ll->length;
        d.sizeX = strlen((char*)ll->head->data);
        cell grid[d.sizeY][d.sizeX];
        cell endGrid[d.sizeY][d.sizeX];
        entry start = {0, 0, EAST};
        entry end = {0, 0, EAST};

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < d.sizeX; x++) {
                        cell *cur = &grid[line][x];
                        cell *endCur = &endGrid[line][x];
                        initCell(cur);
                        initCell(endCur);
                        cur->type = str[x];
                        endCur->type = str[x];
                        if (str[x] == 'S') {
                                start.x = x;
                                start.y = line;
                                cur->dist[EAST] = 0;
                                cur->onPath = true;
                                endCur->onPath = true;
                        } else if (str[x] == 'E') {
                                end.x = x;
                                end.y = line;
                                memset(endCur->dist, 0, sizeof(endCur->dist));
                                endCur->onPath = true;
                                cur->onPath = true;
                        }
                }

                current = current->next;
                line++;
        }
        // printGrid(d, grid);
        printf("Start: (%d, %d)\n", start.x, start.y);

        long endVal = dijkstra(d, grid, start, end, false);

        dijkstra(d, endGrid, end, start, true);
        int numOnPath = backTrace(d, grid, endGrid, endVal);

        // printDist(d, grid);
        // printDist(d, endGrid);
        // printPath(d, grid);

        printf("Part 1: Lowest Score: %ld\n\n", endVal);
        printf("Part 2: Tiles on Path: %d\n\n", numOnPath);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day16.txt");
        else
                ll = getInputFile("assets/inputs/2024/Day16.txt");
        // llist_print(ll, printInput);

        part1_2(ll);

        return 0;
}
