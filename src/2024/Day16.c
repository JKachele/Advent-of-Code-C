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
        long dist;
        bool visited;
        bool onPath;
        direction dir;
} cell;

typedef tll(ivec2) ivec2ll;

void printGrid(data d, cell grid[d.sizeY][d.sizeX]) {
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        printf("%c", grid[y][x].type);
                }
                printf("\n");
        }
        printf("\n");
}

void printDist(data d, cell grid[d.sizeY][d.sizeX]) {
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        if (grid[y][x].type == '#')
                                printf("#### ");
                        else if (grid[y][x].dist == LONG_MAX)
                                printf("  .  ");
                        else if (grid[y][x].dist < 10)
                                printf("  %ld  ", grid[y][x].dist);
                        else if (grid[y][x].dist < 100)
                                printf(" %ld  ", grid[y][x].dist);
                        else if (grid[y][x].dist < 1000)
                                printf(" %ld ", grid[y][x].dist);
                        else
                                printf("%ld ", grid[y][x].dist);
                }
                printf("\n");
        }
        printf("\n");
}

void printPath(data d, cell grid[d.sizeY][d.sizeX]) {
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        if (grid[y][x].type == '#')
                                printf(".");
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

ivec2 lowestDist(data d, cell grid[d.sizeY][d.sizeX], ivec2ll *cells) {
        long minDist = LONG_MAX;
        ivec2 minCell;

        tll_foreach(*cells, it) {
                ivec2 pos = it->item;
                long dist = grid[pos.y][pos.x].dist;
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

int isOpenCell(data d, cell grid[d.sizeY][d.sizeX], ivec2 cell) {
        if (cell.x < 0 || cell.y < 0 || cell.x >= d.sizeX || cell.y >= d.sizeY)
                return 0;
        if (grid[cell.y][cell.x].type == '#')
                return 0;
        return 1;
}

void evalCell(data d, cell grid[d.sizeY][d.sizeX],
                ivec2ll *openCells, ivec2 curPos, int dirOffset) {
        ivec2 dirs[4] = {{0,-1},{1,0},{0,1},{-1,0}};

        direction cellDir = grid[curPos.y][curPos.x].dir;
        long dist = grid[curPos.y][curPos.x].dist;
        int addDist;
        if (dirOffset == 0)
                addDist = STRAIGHT;
        else
                addDist = TURN;

        direction newDir = (cellDir + dirOffset) % 4;
        ivec2 nextPos = addIVec2(curPos, dirs[newDir]);
        cell *next;
        if (isOpenCell(d, grid, nextPos))
                next = &grid[nextPos.y][nextPos.x];
        else return;

        if (next->dist == LONG_MAX) {
                tll_push_back(*openCells, nextPos);
        }
        if (next->dist > dist + addDist) {
                next->dist = dist + addDist;
                next->dir = newDir;
        }
}

void evalAllDir(data d, cell grid[d.sizeY][d.sizeX],
                ivec2ll *openCells, ivec2 curPos) {
        for (int i = 0; i < 4; i++) {
                grid[curPos.y][curPos.x].dir = i;
                evalCell(d, grid, openCells, curPos, 0);       // Straight
                evalCell(d, grid, openCells, curPos, 1);       // Right
                evalCell(d, grid, openCells, curPos, 3);       // Left
        }
        // printDist(d, grid);
}

int dijkstra(data d, cell grid[d.sizeY][d.sizeX],
                ivec2 start, ivec2 end, bool backtrack) {
        ivec2ll openCells = tll_init();

        if (backtrack)
                evalAllDir(d, grid, &openCells, start);
        else
                tll_push_back(openCells, start);

        // Main Dijkstra loop
        while (tll_length(openCells) > 0) {
                // printf("Num Open cells: %zu\n", tll_length(openCells));
                
                // Find cell with lowest distance
                ivec2 curPos = lowestDist(d, grid, &openCells);

                // Set cell to visited;
                grid[curPos.y][curPos.x].visited = true;

                // Analyze adjacent cells
                evalCell(d, grid, &openCells, curPos, 0);       // Straight
                evalCell(d, grid, &openCells, curPos, 1);       // Right
                evalCell(d, grid, &openCells, curPos, 3);       // Left
        }

        return grid[end.y][end.x].dist;
}

int backTrace(data d, cell startGrid[d.sizeY][d.sizeX],
                cell endGrid[d.sizeY][d.sizeX], long minDist) {
        int numOnPath = 0;
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        if (startGrid[y][x].type != '.')
                                continue;
                        int dist = startGrid[y][x].dist + endGrid[y][x].dist;
                        if (dist == minDist) {
                                numOnPath++;
                                startGrid[y][x].onPath = true;
                                endGrid[y][x].onPath = true;
                        } else if (labs(dist - minDist) == TURN - 1) {
                                numOnPath++;
                                startGrid[y][x].onPath = true;
                                endGrid[y][x].onPath = true;
                        }
                }
        }
        return numOnPath + 2;   // Add 2 for start and end cells
}

void initCell(cell *c) {
        c->type = 0;
        c->dist = LONG_MAX;
        c->visited = false;
        c->onPath = false;
        c->dir = EAST;
}

void part1_2(llist *ll) {
        data d;
        d.sizeY = ll->length;
        d.sizeX = strlen((char*)ll->head->data);
        cell grid[d.sizeY][d.sizeX];
        cell endGrid[d.sizeY][d.sizeX];
        ivec2 start = {0, 0};
        ivec2 end = {0, 0};

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < d.sizeX; x++) {
                        initCell(&grid[line][x]);
                        initCell(&endGrid[line][x]);
                        grid[line][x].type = str[x];
                        endGrid[line][x].type = str[x];
                        if (str[x] == 'S') {
                                start = (ivec2){x, line};
                                grid[line][x].dist = 0;
                                grid[line][x].onPath = true;
                                endGrid[line][x].onPath = true;
                        } else if (str[x] == 'E') {
                                end = (ivec2){x, line};
                                endGrid[line][x].dist = 0;
                                endGrid[line][x].onPath = true;
                                grid[line][x].onPath = true;
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
        printPath(d, grid);

        printf("Part 1: Lowest Score: %ld\n\n", endVal);
        printf("Part 2: Tiles on Path: %d\n\n", numOnPath);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/test2.txt");
        else
                ll = getInputFile("assets/2024/Day16.txt");
        // llist_print(ll, printInput);

        part1_2(ll);

        return 0;
}
