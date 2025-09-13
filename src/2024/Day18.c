/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Feb 06, 2025 14:10:47 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"
#include "../util/vector.h"

typedef tll(ivec2) ivectll;

typedef struct {
        int status; // 0: Blocked, 1: open, 2: seen
        int dist;
}cell;

static int gridSize;

void printIvectll(ivectll i) {
        tll_foreach(i, it) {
                printf("[%d, %d]\n", it->item.x, it->item.y);
        }

}

void printGrid(cell grid[gridSize][gridSize]) {
        for (int y = 0; y < gridSize; y++) {
                for (int x = 0; x < gridSize; x++) {
                        printf("%c", grid[y][x].status == 0 ? '#' : '.');
                }
                puts("");
        }
}

bool isValid(ivec2 pos) {
        return (pos.x >= 0 && pos.y >= 0 &&
                pos.x < gridSize && pos.y < gridSize);
}

ivec2 lowestDist(cell grid[gridSize][gridSize], ivectll *cells) {
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

int dijkstra(cell grid[gridSize][gridSize]) {
        ivec2 dirs[4] = {{{0,-1}},{{1,0}},{{0,1}},{{-1,0}}};

        ivectll openCells = tll_init();
        ivec2 start = {0};
        tll_push_back(openCells, start);

        while (tll_length(openCells) > 0) {
                ivec2 cur = lowestDist(grid, &openCells);

                if (cur.x == gridSize - 1 && cur.y == gridSize - 1)
                        break;

                int dist = grid[cur.y][cur.x].dist + 1;

                for (int i = 0; i < 4; i++) {
                        ivec2 new;
                        new.x = cur.x + dirs[i].x;
                        new.y = cur.y + dirs[i].y;
                        if (isValid(new)) {
                                cell *newCell = &grid[new.y][new.x];
                                if (newCell->status == 0)
                                        continue;
                                if (newCell->dist > dist)
                                        newCell->dist = dist;
                                if (newCell->status != 2) {
                                        newCell->status = 2;
                                        tll_push_back(openCells, new);
                                }
                        }
                }

        }

        return grid[gridSize-1][gridSize-1].dist;
}

void resetGrid(cell grid[gridSize][gridSize]) {
        for (int y = 0; y < gridSize; y++) {
                for (int x = 0; x < gridSize; x++) {
                        grid[y][x].dist = INT_MAX;
                        if (grid[y][x].status == 2)
                                grid[y][x].status = 1;
                }
        }
        grid[0][0].dist = 0;
}

void part1(llist *ll) {
        ivectll bytes = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                int x = strtol(strtok(str, ","), (char**)NULL, 10);
                int y = strtol(strtok(NULL, ""), (char**)NULL, 10);
                ivec2 pos = {{x, y}};
                tll_push_back(bytes, pos);
                current = current->next;
        }
        // printIvectll(bytes);

        cell mem[gridSize][gridSize];
        for (int y = 0; y < gridSize; y++) {
                for (int x = 0; x < gridSize; x++) {
                        mem[y][x].status = 1;
                        mem[y][x].dist = INT_MAX;
                }
        }
        mem[0][0].dist = 0;

        // Drop the 1024 bytes onto the grid
        int count = 0;
        tll_foreach(bytes, it) {
                if (gridSize == 71 && count >= 1024)
                        break;
                if (gridSize == 7 && count >= 12)
                        break;
                ivec2 pos = it->item;
                mem[pos.y][pos.x].status = 0;
                count++;
        }
        // printGrid(mem);

        int dist = dijkstra(mem);

        printf("Part 1: Lowest Dist: %d\n\n", dist);
}

void part2(llist *ll) {
        ivectll bytes = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                int x = strtol(strtok(str, ","), (char**)NULL, 10);
                int y = strtol(strtok(NULL, ""), (char**)NULL, 10);
                ivec2 pos = {{x, y}};
                tll_push_back(bytes, pos);
                current = current->next;
        }
        // printIvectll(bytes);

        cell mem[gridSize][gridSize];
        for (int y = 0; y < gridSize; y++) {
                for (int x = 0; x < gridSize; x++) {
                        mem[y][x].status = 1;
                        mem[y][x].dist = INT_MAX;
                }
        }
        mem[0][0].dist = 0;

        // Drop the 1024 bytes onto the grid
        int count = 0;
        ivec2 byte = {0};
        tll_foreach(bytes, it) {
                ivec2 pos = it->item;
                mem[pos.y][pos.x].status = 0;
                count++;
                if (gridSize == 71 && count < 1024)
                        continue;
                if (gridSize == 7 && count < 12)
                        continue;
                resetGrid(mem);
                int dist = dijkstra(mem);
                if (dist == INT_MAX) {
                        byte = pos;
                        break;
                }
        }


        printf("Part 2: Blocking Pos: (%d, %d)\n\n", byte.x, byte.y);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2024/Day18.txt");
                gridSize = 7;
        } else {
                ll = getInputFile("assets/inputs/2024/Day18.txt");
                gridSize = 71;
        }
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
