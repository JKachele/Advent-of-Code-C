/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Feb 11, 2025 15:39:38 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"
#include "../util/vector.h"

#define RANGE 20

typedef enum direction {
        NORTH,
        EAST,
        SOUTH,
        WEST
} direction;

typedef tll(ivec2) ivectll;

typedef struct {
        int x;
        int y;
        ivec2 start;
        ivec2 end;
        int pathLen;
} data;

typedef struct {
        int status; // 0 = blocked, 1 = open, 2 = visited
        int dist;
        ivec2 next;
} cell;

typedef struct {
        int x;
        int y;
        int dist;
} cheat;

typedef tll(cheat) cheatll;

void printMaze(data d, cell maze[d.y][d.x]) {
        for (int y = 0; y < d.y; y++) {
                for (int x = 0; x < d.x; x++) {
                        cell c = maze[y][x];
                        if (c.status == 0)
                                printf("#");
                        else
                                printf(".");
                }
                printf("\n");
        }
}

bool isValid(data d, ivec2 pos) {
        return (pos.x >= 0 && pos.y >= 0 &&
                pos.x < d.x && pos.y < d.y);
}

ivec2 lowestDist(data d, cell grid[d.y][d.x], ivectll *cells) {
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

int dijkstra(data d, cell grid[d.y][d.x]) {
        ivec2 dirs[4] = {{{0,-1}},{{1,0}},{{0,1}},{{-1,0}}};

        ivectll openCells = tll_init();
        tll_push_back(openCells, d.end);

        while (tll_length(openCells) > 0) {
                ivec2 cur = lowestDist(d, grid, &openCells);

                int dist = grid[cur.y][cur.x].dist + 1;

                for (int i = 0; i < 4; i++) {
                        ivec2 new;
                        new.x = cur.x + dirs[i].x;
                        new.y = cur.y + dirs[i].y;
                        if (isValid(d, new)) {
                                cell *newCell = &grid[new.y][new.x];
                                if (newCell->status == 0)
                                        continue;
                                if (newCell->dist > dist) {
                                        newCell->dist = dist;
                                        newCell->next = cur;
                                }
                                if (newCell->status != 2) {
                                        newCell->status = 2;
                                        tll_push_back(openCells, new);
                                }
                        }
                }

        }

        return grid[d.start.y][d.start.x].dist;
}

int timeSaved(data d, cell maze[d.y][d.x], ivec2 pos, direction dir) {
        ivec2 dirs[4] = {{{0,-1}},{{1,0}},{{0,1}},{{-1,0}}};
        // valid cheat: crosses a wall, ends on open space,
        // Landing cell is at least 3 cells closer to end
        ivec2 jmp1 = ivec2Add(pos, dirs[dir]);
        ivec2 land = ivec2Add(jmp1, dirs[dir]);
        if (!isValid(d, jmp1) || !isValid(d, land))
                return -1;

        // Doesn't cross wall
        if (maze[jmp1.y][jmp1.x].status != 0)
                return -1;

        // Doesn't land on open space
        if (maze[land.y][land.x].status == 0)
                return -1;

        // doesn't save time
        int timeSaved = maze[pos.y][pos.x].dist - maze[land.y][land.x].dist - 2;
        if (timeSaved < 1)
                return -1;

        return timeSaved;
}

int findCheatsP1(data d, cell maze[d.y][d.x], int cheats[]) {
        int numCheats = 0;
        ivec2 cur = d.start;
        while (!ivec2Eq(cur, d.end)) {
                for (int i = 0; i < 4; i++) {
                        int time = timeSaved(d, maze, cur, i);
                        if (time == -1)
                                continue;
                        cheats[time]++;
                        if (time >= 100)
                                numCheats++;
                }
                cur = maze[cur.y][cur.x].next;
        }
        return numCheats;
}

int manhattanDist(ivec2 a, ivec2 b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
}

int getCellsInRange(data d, ivec2 pos, cheatll *inRange) {
        int numInRange = 0;
        for (int y = 0; y < d.y; y++) {
                for (int x = 0; x < d.x; x++) {
                        ivec2 test = {{x, y}};
                        int mDist = manhattanDist(pos, test);
                        cheat testC = {x, y, mDist};
                        if (mDist <= RANGE) {
                                tll_push_back(*inRange, testC);
                                numInRange++;
                        }
                }
        }
        return numInRange;
}

int countCheats(data d, cell maze[d.y][d.x], int cheats[],
                ivec2 pos, cheatll inRange) {
        int dist = maze[pos.y][pos.x].dist;
        int numCheats = 0;
        tll_foreach(inRange, it) {
                cheat testPos = it->item;
                int testDist = maze[testPos.y][testPos.x].dist;
                if (testDist == INT_MAX) continue;
                int timeSaved = (dist - testDist) - testPos.dist;
                if (timeSaved >= 100) {
                        cheats[timeSaved]++;
                        numCheats++;
                }
        }
        return numCheats;
}

int findCheatsP2(data d, cell maze[d.y][d.x], int cheats[]) {
        int numCheats = 0;

        ivec2 cur = d.start;
        while (!ivec2Eq(cur, d.end)) {
                cheatll inRange = tll_init();
                getCellsInRange(d, cur, &inRange);
                numCheats += countCheats(d, maze, cheats, cur, inRange);
                cur = maze[cur.y][cur.x].next;
        }
        return numCheats;
}

void initCell(cell *c, char type) {
        c->status = (type == '#') ? 0 : 1;
        c->dist = (type == 'E') ? 0 : INT_MAX;
        c->next.x = -1;
        c->next.y = -1;
}

void part1(llist *ll) {
        data d = {0};
        d.x = getLongestLine(ll);
        d.y = ll->length;
        cell maze[d.y][d.x];

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char *str = (char*)current->data;
                for (int x = 0; x < (int)strlen(str); x++) {
                        initCell(&maze[line][x], str[x]);
                        if (str[x] == 'S') {
                                d.start.x = x;
                                d.start.y = line;
                        } else if (str[x] == 'E') {
                                d.end.x = x;
                                d.end.y = line;
                        }
                }
                current = current->next;
                line++;
        }
        // printMaze(d, maze);

        d.pathLen = dijkstra(d, maze);
        int cheats[d.pathLen];
        for (int i = 0; i < d.pathLen; i++) { cheats[i] = 0; }

        int numCheats = findCheatsP1(d, maze, cheats);
        // for (int i = 0; i < d.pathLen; i++) {
        //         if (cheats[i] > 0)
        //                 printf("%d: %d\n", i, cheats[i]);
        // }

        printf("Part 1: Num Cheats: %d\n\n", numCheats);
}

void part2(llist *ll) {
        data d = {0};
        d.x = getLongestLine(ll);
        d.y = ll->length;
        cell maze[d.y][d.x];

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char *str = (char*)current->data;
                for (int x = 0; x < (int)strlen(str); x++) {
                        initCell(&maze[line][x], str[x]);
                        if (str[x] == 'S') {
                                d.start.x = x;
                                d.start.y = line;
                        } else if (str[x] == 'E') {
                                d.end.x = x;
                                d.end.y = line;
                        }
                }
                current = current->next;
                line++;
        }
        // printMaze(d, maze);

        d.pathLen = dijkstra(d, maze);
        int cheats[d.pathLen];
        for (int i = 0; i < d.pathLen; i++) { cheats[i] = 0; }

        int numCheats = findCheatsP2(d, maze, cheats);
        // for (int i = 50; i < d.pathLen; i++) {
        //         if (cheats[i] > 0)
        //                 printf("%d - %d\n", i, cheats[i]);
        // }

        printf("Part 2: Num Cheats: %d\n\n", numCheats);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day20.txt");
        } else {
                ll = getInputFile("assets/inputs/2024/Day20.txt");
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

