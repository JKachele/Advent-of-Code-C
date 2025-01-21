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

typedef struct {
        int sizeX;
        int sizeY;
} data;

typedef struct {
        char type;
        int dist;
        bool visited;
        direction dir;
} cell;

void printGrid(data d, cell grid[d.sizeY][d.sizeX]) {
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        printf("%c", grid[y][x].type);
                }
                printf("\n");
        }
}

llNode *lowestDist(data d, cell grid[d.sizeY][d.sizeX], llist *cells) {
        int minDist = INT_MAX;
        llNode *minCell;
        llNode *cur = cells->head;
        while (cur != NULL) {
                ivec2 *pos = (ivec2*)cur->data;
                printf("1pos: %d, %d\n", pos->x, pos->y);
                if (grid[pos->y][pos->x].dist < minDist) {
                        minDist = grid[pos->y][pos->x].dist;
                        minCell = cur;
                        // ivec2 pos2 = *(ivec2*)cur->data;
                        // printf("2pos: %ld, %ld\n", pos2.x, pos2.y);
                }
                cur = cur->next;
        }
        // ivec2 pos = *(ivec2*)minCell->data;
        // printf("3pos: %ld, %ld\n", pos->x, pos->y);
        return minCell;
}

bool isOpenCell(data d, cell grid[d.sizeY][d.sizeX], ivec2 cell) {
        if (cell.x < 0 || cell.y < 0 || cell.x >= d.sizeX || cell.y >= d.sizeY)
                return false;
        if (grid[cell.y][cell.x].type == '#')
                return false;
        if (grid[cell.y][cell.x].visited)
                return false;
        return true;
}

int dijkstra(data d, cell grid[d.sizeY][d.sizeX], ivec2 start) {
        ivec2 dirs[4] = {{0,-1},{1,0},{0,1},{-1,0}};

        llist *openCells = llist_create();
        ivec2 *s = malloc(sizeof(ivec2));
        s->x = start.x;
        s->y = start.y;
        llist_add(openCells, s);

        // Main Dijkstra loop
        while (openCells->length > 0) {
                printf("Num Open cells: %d\n", openCells->length);
                // Find cell with lowest distance
                llNode *cur = lowestDist(d, grid, openCells);
                ivec2 pos = *(ivec2*)cur->data;
                direction cellDir = grid[pos.y][pos.x].dir;

                // Analyze adjacent cells
                // Cell in front
                ivec2 *next = malloc(sizeof(ivec2));
                *next = addIVec2(pos, dirs[cellDir]);
                if (isOpenCell(d, grid, *next)) {
                        llist_add(openCells, next);
                }

                llist_remove_node(openCells, cur);
        }

        return 0;
}

void part1(llist *ll) {
        data d;
        d.sizeY = ll->length;
        d.sizeX = strlen((char*)ll->head->data);
        cell grid[d.sizeY][d.sizeX];
        ivec2 start = {0, 0};

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < d.sizeX; x++) {
                        grid[line][x].type = str[x];
                        grid[line][x].dist = INT_MAX;
                        grid[line][x].visited = false;
                        grid[line][x].dir = EAST;
                        if (str[x] == 'S') {
                                start = (ivec2){x, line};
                                grid[line][x].dist = 0;
                        }
                }

                current = current->next;
                line++;
        }
        // printGrid(d, grid);
        printf("Start: (%d, %d)\n", start.x, start.y);
        dijkstra(d, grid, start);

        printf("Part 1: \n\n");
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/test.txt");
        else
                ll = getInputFile("assets/2024/Day16.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
