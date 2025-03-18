/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Jan 07, 2025 13:46:18 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
// #include "../util/util.h"

struct data {
        int x;
        int y;
        int area;
        int walls;
};

typedef struct {
        char type;
        int visited;
} plant;

bool isValidMove(struct data *d, int x, int y, char key,
                plant field[d->y][d->x]) {
        if (x < 0 || y < 0 || x >= d->x || y >= d->y)
                return false;
        if (field[y][x].type != key)
                return false;
        return true;
}

void bfs(struct data *d, int x, int y, plant field[d->y][d->x]) {
        if (field[y][x].visited == 1)
                return;
        field[y][x].visited = 1;
        d->area++;

        // Move direction arrays
        //             R,  D,  L,  U
        int moveX[] = {1,  0, -1,  0};
        int moveY[] = {0,  1,  0, -1};

        char plant = field[y][x].type;
        for (int i = 0; i < 4; i++) {
                int newX = x + moveX[i];
                int newY = y + moveY[i];
                if (isValidMove(d, newX, newY, plant, field)) {
                        bfs(d, newX, newY, field);
                } else {
                        d->walls++;
                }
        }
}

int adjacent2(struct data *d, int x, int y,
                plant field[d->y][d->x], bool adjacent[4]) {
        // Straight line
        if ((adjacent[0] && adjacent[2]) ||
                        adjacent[1] && adjacent[3]) {
                return 0;
        }

        int cornerX[] = {1, 1, -1, -1};
        int cornerY[] = {-1, 1, 1, -1};

        // Check for plants in the corner of adjacent plants
        // if corner is different, 2 corners, if it is the same, 1 corner
        for (int i = 0; i < 4; i++) {
                // Mod 4 for upper left corner 4 % 4 = 0
                if (adjacent[i] && adjacent[(i+1)%4]) {
                        plant corner = field[y + cornerY[i]][x + cornerX[i]];
                        if (corner.type != field[y][x].type)
                                return 2;
                        return 1;
                }
        }
        return -1;
}

int adjacent3(struct data *d, int x, int y,
                plant field[d->y][d->x], bool adjacent[4]) {
        char type = field[y][x].type;
        int cornerX[] = {1, 1, -1, -1};
        int cornerY[] = {-1, 1, 1, -1};

        for (int i = 0; i < 4; i++) {
                if (adjacent[i])
                        continue;

                int corner1x = x + cornerX[(i + 1) % 4];
                int corner1y = y + cornerY[(i + 1) % 4];
                int corner2x = x + cornerX[(i + 2) % 4];
                int corner2y = y + cornerY[(i + 2) % 4];
                char corner1 = field[corner1y][corner1x].type;
                char corner2 = field[corner2y][corner2x].type;

                int numCorners = 0;
                if (corner1 != type)
                        numCorners ++;
                if (corner2 != type)
                        numCorners ++;
                return numCorners;
        }
        return -1;
}

int adjacent4(struct data *d, int x, int y,
                plant field[d->y][d->x], bool adjacent[4]) {
        char type = field[y][x].type;
        int cornerX[] = {1, 1, -1, -1};
        int cornerY[] = {-1, 1, 1, -1};

        int numCorners = 0;
        for (int i = 0; i < 4; i++) {
                if (field[y + cornerY[i]][x + cornerX[i]].type != type)
                        numCorners++;
        }
        return numCorners;
}

int countCorners(struct data *d, int x, int y, plant field[d->y][d->x]) {
        // Direction arrays
        //              U,  R,  D,  L
        int moveX[] = { 0,  1,  0, -1};
        int moveY[] = {-1,  0,  1,  0};

        bool adjacent[4] = {0};
        int numAdjacent = 0;
        for (int i = 0; i < 4; i++) {
                int newX = x + moveX[i];
                int newY = y + moveY[i];
                if (isValidMove(d, newX, newY, field[y][x].type, field) == 1) {
                        adjacent[i] = true;
                        numAdjacent++;
                }
        }

        switch (numAdjacent) {
        case 0:
                return 4;
        case 1:
                return 2;
        case 2:
                return adjacent2(d, x, y, field, adjacent);
        case 3:
                return adjacent3(d, x, y, field, adjacent);
        case 4:
                return adjacent4(d, x, y, field, adjacent);
        default:
                return -1;
        }
}

void bfs2(struct data *d, int x, int y, plant field[d->y][d->x]) {
        if (field[y][x].visited == 1)
                return;
        field[y][x].visited = 1;
        d->area++;

        // Move direction arrays
        //              U,  R,  D,  L
        int moveX[] = { 0,  1,  0, -1};
        int moveY[] = {-1,  0,  1,  0};

        d->walls += countCorners(d, x, y, field);

        char plant = field[y][x].type;
        for (int i = 0; i < 4; i++) {
                int newX = x + moveX[i];
                int newY = y + moveY[i];
                if (isValidMove(d, newX, newY, plant, field) == 1) {
                        bfs2(d, newX, newY, field);
                }
        }
}

void part1(llist *ll) {
        struct data d = {0, 0, 0, 0}; 
        d.y = ll->length;
        d.x = getLongestLine(ll);
        plant field[d.y][d.x];

        llNode *current = ll->head;
        int row = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int i = 0; i < d.x; i++) {
                        field[row][i].type = str[i];
                        field[row][i].visited = 0;
                }
                current = current->next;
                row++;
        }

        int price = 0;
        for (int y = 0; y < d.y; y++) {
                for (int x = 0; x < d.x; x++) {
                        if (field[y][x].visited == 1)
                                continue;

                        d.area = 0;
                        d.walls = 0;

                        bfs(&d, x, y, field);
                        // printf("%c - Area: %d, Perim: %d\n",
                        //                 field[y][x].type, d.area, d.walls);
                        price += d.area * d.walls;
                }
        }

        printf("Part 1: Total Price: %d\n\n", price);
}

void part2(llist *ll) {
        struct data d = {0, 0, 0, 0}; 
        d.y = ll->length;
        d.x = getLongestLine(ll);
        plant field[d.y][d.x];

        llNode *current = ll->head;
        int row = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int i = 0; i < d.x; i++) {
                        field[row][i].type = str[i];
                        field[row][i].visited = 0;
                }
                current = current->next;
                row++;
        }
        
        int price = 0;
        for (int y = 0; y < d.y; y++) {
                for (int x = 0; x < d.x; x++) {
                        if (field[y][x].visited == 1)
                                continue;

                        d.area = 0;
                        d.walls = 0;

                        bfs2(&d, x, y, field);
                        // printf("%c - Area: %d, Perim: %d\n",
                        //                 field[y][x].type, d.area, d.walls);
                        price += d.area * d.walls;
                }
        }

        printf("Part 1: Total Price: %d\n\n", price);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day12.txt");
        else
                ll = getInputFile("assets/inputs/2024/Day12.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
