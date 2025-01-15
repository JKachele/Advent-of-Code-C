/*************************************************
 *File----------day6.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Dec 17, 2024 09:14:28 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"

typedef struct {
        bool up;
        bool right;
        bool down;
        bool left;
} cell;

int MAX_Y = 0;
int MAX_X = 0;

void printMap(char map[MAX_Y][MAX_X]) {
        for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        printf("[%c]", map[y][x]);
                }
                printf("\n");
        }
}

void clearCells(cell cells[MAX_Y][MAX_X]) {
        for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        cells[y][x] = (cell){0, 0, 0, 0};
                }
        }
}

int validPos(char map[MAX_Y][MAX_X], vector2 pos) {
        if (pos.x < 0 || pos.y < 0)
                return -1;
        if (pos.x >= MAX_X || pos.y >= MAX_Y)
                return -1;
        if (map[pos.y][pos.x] == '#')
                return 1;
        return 0;
}

vector2 incrementMap(char map[MAX_Y][MAX_X], vector2 pos) {
        int valid;
        vector2 newPos;
        switch (map[pos.y][pos.x]) {
        case '^':
                newPos = (vector2){pos.x, pos.y - 1};
                valid = validPos(map, (vector2){newPos.x, newPos.y});
                if (valid == 0) {
                        map[pos.y][pos.x] = 'X';
                        map[newPos.y][newPos.x] = '^';
                        return (vector2){newPos.x, newPos.y};
                } else if (valid == 1) {
                        map[pos.y][pos.x] = '>';
                        return incrementMap(map, pos);
                } else {
                        map[pos.y][pos.x] = 'X';
                        return (vector2){-1, -1};
                }
        case '>':
                newPos = (vector2){pos.x + 1, pos.y};
                valid = validPos(map, (vector2){newPos.x, newPos.y});
                if (valid == 0) {
                        map[pos.y][pos.x] = 'X';
                        map[newPos.y][newPos.x] = '>';
                        return (vector2){newPos.x, newPos.y};
                } else if (valid == 1) {
                        map[pos.y][pos.x] = 'V';
                        return incrementMap(map, pos);
                } else {
                        map[pos.y][pos.x] = 'X';
                        return (vector2){-1, -1};
                }
        case 'V':
                newPos = (vector2){pos.x, pos.y + 1};
                valid = validPos(map, (vector2){newPos.x, newPos.y});
                if (valid == 0) {
                        map[pos.y][pos.x] = 'X';
                        map[newPos.y][newPos.x] = 'V';
                        return (vector2){newPos.x, newPos.y};
                } else if (valid == 1) {
                        map[pos.y][pos.x] = '<';
                        return incrementMap(map, pos);
                } else {
                        map[pos.y][pos.x] = 'X';
                        return (vector2){-1, -1};
                }
        case '<':
                newPos = (vector2){pos.x - 1, pos.y};
                valid = validPos(map, (vector2){newPos.x, newPos.y});
                if (valid == 0) {
                        map[pos.y][pos.x] = 'X';
                        map[newPos.y][newPos.x] = '<';
                        return (vector2){newPos.x, newPos.y};
                } else if (valid == 1) {
                        map[pos.y][pos.x] = '^';
                        return incrementMap(map, pos);
                } else {
                        map[pos.y][pos.x] = 'X';
                        return (vector2){-1, -1};
                }
        default:
                fprintf(stderr, "Error Invalid Position Char \'%c",
                                map[pos.y][pos.x]);
                return (vector2){-1, -1};
        }
}

// dir: 0:Up, 1:Right, 2:Down, 3:Left
int addCell(cell cells[MAX_Y][MAX_X], vector2 pos, int dir) {
        switch (dir) {
        case 0:
                if (cells[pos.y][pos.x].up) {
                        return 1;
                } else {
                        cells[pos.y][pos.x].up = true;
                        return 0;
                }
        case 1:
                if (cells[pos.y][pos.x].right) {
                        return 1;
                } else {
                        cells[pos.y][pos.x].right = true;
                        return 0;
                }
        case 2:
                if (cells[pos.y][pos.x].down) {
                        return 1;
                } else {
                        cells[pos.y][pos.x].down = true;
                        return 0;
                }
        case 3:
                if (cells[pos.y][pos.x].left) {
                        return 1;
                } else {
                        cells[pos.y][pos.x].left = true;
                        return 0;
                }
        default:
                return -1;
        }
}

vector2 incrementRecordMap(char map[MAX_Y][MAX_X], vector2 pos,
                cell cells[MAX_Y][MAX_X]) {
        int valid;
        vector2 newPos;
        switch (map[pos.y][pos.x]) {
        case '^':
                newPos = (vector2){pos.x, pos.y - 1};
                valid = validPos(map, (vector2){newPos.x, newPos.y});
                if (valid == 0) {
                        map[pos.y][pos.x] = 'X';
                        if (addCell(cells, newPos, 0) == 1) {
                                return (vector2){-2, -2};
                        }
                        map[newPos.y][newPos.x] = '^';
                        return (vector2){newPos.x, newPos.y};
                } else if (valid == 1) {
                        map[pos.y][pos.x] = '>';
                        return incrementRecordMap(map, pos, cells);
                } else {
                        map[pos.y][pos.x] = 'X';
                        return (vector2){-1, -1};
                }
        case '>':
                newPos = (vector2){pos.x + 1, pos.y};
                valid = validPos(map, (vector2){newPos.x, newPos.y});
                if (valid == 0) {
                        map[pos.y][pos.x] = 'X';
                        if (addCell(cells, newPos, 1) == 1) {
                                return (vector2){-2, -2};
                        }
                        map[newPos.y][newPos.x] = '>';
                        return (vector2){newPos.x, newPos.y};
                } else if (valid == 1) {
                        map[pos.y][pos.x] = 'V';
                        return incrementRecordMap(map, pos, cells);
                } else {
                        map[pos.y][pos.x] = 'X';
                        return (vector2){-1, -1};
                }
        case 'V':
                newPos = (vector2){pos.x, pos.y + 1};
                valid = validPos(map, (vector2){newPos.x, newPos.y});
                if (valid == 0) {
                        map[pos.y][pos.x] = 'X';
                        if (addCell(cells, newPos, 2) == 1) {
                                return (vector2){-2, -2};
                        }
                        map[newPos.y][newPos.x] = 'V';
                        return (vector2){newPos.x, newPos.y};
                } else if (valid == 1) {
                        map[pos.y][pos.x] = '<';
                        return incrementRecordMap(map, pos, cells);
                } else {
                        map[pos.y][pos.x] = 'X';
                        return (vector2){-1, -1};
                }
        case '<':
                newPos = (vector2){pos.x - 1, pos.y};
                valid = validPos(map, (vector2){newPos.x, newPos.y});
                if (valid == 0) {
                        map[pos.y][pos.x] = 'X';
                        if (addCell(cells, newPos, 3) == 1) {
                                return (vector2){-2, -2};
                        }
                        map[newPos.y][newPos.x] = '<';
                        return (vector2){newPos.x, newPos.y};
                } else if (valid == 1) {
                        map[pos.y][pos.x] = '^';
                        return incrementRecordMap(map, pos, cells);
                } else {
                        map[pos.y][pos.x] = 'X';
                        return (vector2){-1, -1};
                }
        default:
                fprintf(stderr, "Error Invalid Position Char \'%c",
                                map[pos.y][pos.x]);
                return (vector2){-1, -1};
        }
}

void part1(llist *ll) {
        llNode *current = ll->head;
        MAX_Y = ll->length;
        MAX_X = strlen((char*)current->data);
        vector2 curPos = {0, 0};

        char map[MAX_Y][MAX_X];
        int curLine = 0;
        while(current != NULL) {
                // char str[BUFFER_SIZE];
                // strncpy(str, (char*)current->data, BUFFER_SIZE);
                strncpy(map[curLine], (char*)current->data, MAX_X);

                // Look for janator position
                for (int i = 0; i < MAX_X; i++) {
                        if (map[curLine][i] == '^')
                                curPos = (vector2){i, curLine};
                }

                current = current->next;
                curLine++;
        }

        // printf("Current Position: %d, %d\n", curPos.x, curPos.y);
        // printMap(map);

        while (curPos.x != -1) {
                curPos = incrementMap(map, curPos);
        }
        // printf("Current Position: %d, %d\n", curPos.x, curPos.y);
        // printMap(map);

        int positionCount = 0;
        for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        if (map[y][x] == 'X')
                                positionCount++;
                }
        }

        printf("Part 1: Distinct Positions: %d\n\n", positionCount);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        MAX_Y = ll->length;
        MAX_X = strlen((char*)current->data);
        vector2 curPos = {0, 0};

        char map[MAX_Y][MAX_X];
        int curLine = 0;
        while(current != NULL) {
                // char str[BUFFER_SIZE];
                // strncpy(str, (char*)current->data, BUFFER_SIZE);
                strncpy(map[curLine], (char*)current->data, MAX_X);

                // Look for janator position
                for (int i = 0; i < MAX_X; i++) {
                        if (map[curLine][i] == '^')
                                curPos = (vector2){i, curLine};
                }

                current = current->next;
                curLine++;
        }
        vector2 startPos = curPos;

        // Get original Path
        while (curPos.x >= 0) {
                curPos = incrementMap(map, curPos);
        }
        map[startPos.y][startPos.x] = '^';
        // printMap(map);

        vector2 visited[MAX_X * MAX_Y];
        int numVisited = 0;
        for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        if (map[y][x] == 'X') {
                                visited[numVisited] = (vector2){x, y};
                                numVisited++;
                        }
                }
        }
        printf("numVisited: %d\n", numVisited);

        cell cells[MAX_Y][MAX_X];
        clearCells(cells);

        int numNewObs = 0;
        for (int i = 0; i < numVisited; i++) {
                vector2 new = visited[i];
                map[new.y][new.x] = '#';
                while (curPos.x >= 0) {
                        curPos = incrementRecordMap(map, curPos, cells);
                }
                map[new.y][new.x] = '.';
                if (curPos.x == -2)
                        numNewObs++;
                map[startPos.y][startPos.x] = '^';
                curPos = startPos;
                clearCells(cells);
        }

        printf("Part 2: New Obstriction Count: %d\n", numNewObs);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/test.txt");
        else
                ll = getInputFile("assets/2024/Day6.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
