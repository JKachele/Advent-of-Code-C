/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Jan 13, 2025 11:34:28 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
// #include "../util/util.h"

typedef struct {
        int sizeX;
        int sizeY;
        int posX;
        int posY;
} data;

void printGrid(data d, char grid[d.sizeY][d.sizeX]) {
        printf("(%d, %d)\n", d.posX, d.posY);
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        printf("%c", grid[y][x]);
                }
                printf("\n");
        }
}

long getGPS(data d, char grid[d.sizeY][d.sizeX]) {
        long gps = 0;
        for (int y = 0; y < d.sizeY; y++) {
                for (int x = 0; x < d.sizeX; x++) {
                        if (grid[y][x] == 'O' || grid[y][x] == '[')
                                gps += x + (100 * y);
                }
        }
        return gps;
}

bool canMove(data *d, char grid[d->sizeY][d->sizeX], char dir) {
        // Chacks if there is an empty space between the robot
        // and the next wall, if wall is detected first, move not possible
        switch (dir) {
        case '^':
                for (int y = d->posY; y >= 0; y--) {
                        if (grid[y][d->posX] == '#')
                                return false;
                        if (grid[y][d->posX] == '.')
                                return true;
                }
                return false;
        case '>':
                for (int x = d->posX; x < d->sizeX; x++) {
                        if (grid[d->posY][x] == '#')
                                return false;
                        if (grid[d->posY][x] == '.')
                                return true;
                }
                return false;
        case 'v':
                for (int y = d->posY; y < d->sizeY; y++) {
                        if (grid[y][d->posX] == '#')
                                return false;
                        if (grid[y][d->posX] == '.')
                                return true;
                }
                return false;
        case '<':
                for (int x = d->posX; x >= 0; x--) {
                        if (grid[d->posY][x] == '#')
                                return false;
                        if (grid[d->posY][x] == '.')
                                return true;
                }
                return false;
        default:
                return false;
        }
}

int move(data *d, char grid[d->sizeY][d->sizeX], char dir) {
        if (!canMove(d, grid, dir))
                return 0;

        char cur = '.';
        switch (dir) {
        case '^':
                for (int y = d->posY; y >= 0; y--) {
                        if (grid[y][d->posX] == '.') {
                                grid[y][d->posX] = cur;
                                break;
                        }
                        char tmp = grid[y][d->posX];
                        grid[y][d->posX] = cur;
                        cur = tmp;
                }
                d->posY--;
                break;
        case '>':
                for (int x = d->posX; x < d->sizeX; x++) {
                        if (grid[d->posY][x] == '.') {
                                grid[d->posY][x] = cur;
                                break;
                        }
                        char tmp = grid[d->posY][x];
                        grid[d->posY][x] = cur;
                        cur = tmp;
                }
                d->posX++;
                break;
        case 'v':
                for (int y = d->posY; y < d->sizeY; y++) {
                        if (grid[y][d->posX] == '.') {
                                grid[y][d->posX] = cur;
                                break;
                        }
                        char tmp = grid[y][d->posX];
                        grid[y][d->posX] = cur;
                        cur = tmp;
                }
                d->posY++;
                break;
        case '<':
                for (int x = d->posX; x >= 0; x--) {
                        if (grid[d->posY][x] == '.') {
                                grid[d->posY][x] = cur;
                                break;
                        }
                        char tmp = grid[d->posY][x];
                        grid[d->posY][x] = cur;
                        cur = tmp;
                }
                d->posX--;
                break;
        default:
                return -1;
        }

        return 0;
}

int moveUp(data *d, char grid[d->sizeY][d->sizeX], int x, int y) {
        char cur = '.';
        for (int newY = y; newY >= 0; newY--) {
                if (grid[newY][x] == '.') {
                        grid[newY][x] = cur;
                        break;
                }
                if (grid[newY - 1][x] == '[')
                        moveUp(d, grid, x + 1, newY - 1);
                if (grid[newY - 1][x] == ']')
                        moveUp(d, grid, x - 1, newY - 1);
                char tmp = grid[newY][x];
                grid[newY][x] = cur;
                cur = tmp;
        }
        return 0;
}

int moveRight(data *d, char grid[d->sizeY][d->sizeX], int x, int y) {
        char cur = '.';
        for (int newX = d->posX; newX < d->sizeX; newX++) {
                if (grid[d->posY][newX] == '.') {
                        grid[d->posY][newX] = cur;
                        break;
                }
                char tmp = grid[d->posY][newX];
                grid[d->posY][newX] = cur;
                cur = tmp;
        }
        return 0;
}

int moveDown(data *d, char grid[d->sizeY][d->sizeX], int x, int y) {
        char cur = '.';
        for (int newY = y; newY < d->sizeY; newY++) {
                if (grid[newY][x] == '.') {
                        grid[newY][x] = cur;
                        break;
                }
                if (grid[newY + 1][x] == '[')
                        moveDown(d, grid, x + 1, newY + 1);
                if (grid[newY + 1][x] == ']')
                        moveDown(d, grid, x - 1, newY + 1);
                char tmp = grid[newY][x];
                grid[newY][x] = cur;
                cur = tmp;
        }
        return 0;
}

int moveLeft(data *d, char grid[d->sizeY][d->sizeX], int x, int y) {
        char cur = '.';
        for (int newX = d->posX; newX >= 0; newX--) {
                if (grid[d->posY][newX] == '.') {
                        grid[d->posY][newX] = cur;
                        break;
                }
                char tmp = grid[d->posY][newX];
                grid[d->posY][newX] = cur;
                cur = tmp;
        }
        return 0;
}

bool canMove2(data *d, char grid[d->sizeY][d->sizeX], char dir, int x, int y) {
        // Chacks if there is an empty space between the robot
        // and the next wall, if wall is detected first, move not possible
        // Recursive for up and down to check for cascading moves
        switch (dir) {
        case '^':
                if (grid[y][x]== '#')
                        return false;
                if (grid[y][x] == '.')
                        return true;

                if (grid[y - 1][x] == '[') {
                        bool left = canMove2(d, grid, dir, x, y - 1);
                        bool right = canMove2(d, grid, dir, x + 1, y - 1);
                        return left && right;
                } else if (grid[y - 1][x] == ']') {
                        bool left = canMove2(d, grid, dir, x - 1, y - 1);
                        bool right = canMove2(d, grid, dir, x, y - 1);
                        return left && right;
                } else {
                        return canMove2(d, grid, dir, x, y - 1);
                }
        case '>':
                for (int newX = d->posX; newX < d->sizeX; newX++) {
                        if (grid[d->posY][newX] == '#')
                                return false;
                        if (grid[d->posY][newX] == '.')
                                return true;
                }
                return false;
        case 'v':
                if (grid[y][x]== '#')
                        return false;
                if (grid[y][x] == '.')
                        return true;

                if (grid[y + 1][x] == '[') {
                        bool left = canMove2(d, grid, dir, x, y + 1);
                        bool right = canMove2(d, grid, dir, x + 1, y + 1);
                        return left && right;
                } else if (grid[y + 1][x] == ']') {
                        bool left = canMove2(d, grid, dir, x - 1, y + 1);
                        bool right = canMove2(d, grid, dir, x, y + 1);
                        return left && right;
                } else {
                        return canMove2(d, grid, dir, x, y + 1);
                }
        case '<':
                for (int newX = d->posX; newX >= 0; newX--) {
                        if (grid[d->posY][newX] == '#')
                                return false;
                        if (grid[d->posY][newX] == '.')
                                return true;
                }
                return false;
        default:
                return false;
        }
}

int move2(data *d, char grid[d->sizeY][d->sizeX], char dir) {
        if (!canMove2(d, grid, dir, d->posX, d->posY))
                return 0;

        switch (dir) {
        case '^':
                moveUp(d, grid, d->posX, d->posY);
                d->posY--;
                break;
        case '>':
                moveRight(d, grid, d->posX, d->posY);
                d->posX++;
                break;
        case 'v':
                moveDown(d, grid, d->posX, d->posY);
                d->posY++;
                break;
        case '<':
                moveLeft(d, grid, d->posX, d->posY);
                d->posX--;
                break;
        default:
                return -1;
        }

        return 0;
}

void part1(llist *ll) {
        data d = {0, 0, 0, 0};
        d.sizeX = strlen((char*)ll->head->data);
        llNode *current = ll->head;
        while (current != NULL && strlen((char*)current->data) > 0) {
                d.sizeY++;
                current = current->next;
        }
        char grid[d.sizeY][d.sizeX];

        current = ll->head;
        int line = 0;
        while (current != NULL && strlen((char*)current->data) > 0) {
                strncpy(grid[line], (char*)current->data, d.sizeX);

                for (int x = 0; x < d.sizeX; x++) {
                        if (grid[line][x] == '@') {
                                d.posX = x;
                                d.posY = line;
                        }
                }

                current = current->next;
                line++;
        }
        current = current->next;
        // printGrid(d, grid);

        int moveLineLen = strlen((char*)current->data);
        int moveLines = ll->length - (d.sizeY + 1);
        int numMoves = moveLineLen * moveLines;

        char moves[numMoves];
        int offset = 0;
        while(current != NULL) {
                strncpy(moves + offset, (char*)current->data, moveLineLen);
                current = current->next;
                offset += moveLineLen;
        }

        for (int i = 0; i < numMoves; i++) {
                move(&d, grid, moves[i]);
                // printf("%c: ", moves[i]);
                // printGrid(d, grid);
        }
        printGrid(d, grid);

        long gps = getGPS(d, grid);

        printf("Part 1: GPS Sum: %ld\n\n", gps);
}

void part2(llist *ll) {
        data d = {0, 0, 0, 0};
        d.sizeX = strlen((char*)ll->head->data) * 2;
        llNode *current = ll->head;
        while (current != NULL && strlen((char*)current->data) > 0) {
                d.sizeY++;
                current = current->next;
        }
        char grid[d.sizeY][d.sizeX];

        current = ll->head;
        int line = 0;
        while (current != NULL && strlen((char*)current->data) > 0) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                for (int x = 0; x < d.sizeX / 2; x++) {
                        if (str[x] == '@') {
                                grid[line][x * 2] = '@';
                                grid[line][(x * 2) + 1] = '.';
                                d.posX = x * 2;
                                d.posY = line;
                        } else if (str[x] == 'O') {
                                grid[line][x * 2] = '[';
                                grid[line][(x * 2) + 1] = ']';
                        } else {
                                grid[line][x * 2] = str[x];
                                grid[line][(x * 2) + 1] = str[x];
                        }
                }

                current = current->next;
                line++;
        }
        current = current->next;
        printGrid(d, grid);

        int moveLineLen = strlen((char*)current->data);
        int moveLines = ll->length - (d.sizeY + 1);
        int numMoves = moveLineLen * moveLines;

        char moves[numMoves];
        int offset = 0;
        while(current != NULL) {
                strncpy(moves + offset, (char*)current->data, moveLineLen);
                current = current->next;
                offset += moveLineLen;
        }

        for (int i = 0; i < numMoves; i++) {
                move2(&d, grid, moves[i]);
                // printf("%d %c: ", i, moves[i]);
                // printGrid(d, grid);
        }
        printGrid(d, grid);

        long gps = getGPS(d, grid);
        printf("Part 2: GPS Sum: %ld\n", gps);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/test.txt");
        else
                ll = getInputFile("assets/2024/Day15.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
