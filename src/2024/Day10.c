/*************************************************
 *File----------Day10.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Dec 24, 2024 11:56:55 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"

int SIZE_X = 0;
int SIZE_Y = 0;

typedef enum {
        UP,
        RIGHT,
        DOWN,
        LEFT
} dir;

typedef struct {
        int elev;
        bool visit;
} node;

void printMap(node map[SIZE_Y][SIZE_X]) {
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        printf("%d", map[y][x].elev);
                }
                printf("\n");
        }
}

void resetMap(node map[SIZE_Y][SIZE_X]) {
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        map[y][x].visit = false;
                }
        }
}

bool isValidPos(ivec2 pos) {
        return (pos.x >=0 && pos.x < SIZE_X
                        && pos.y >= 0 && pos.y < SIZE_Y);
}

int getNextPos(ivec2 cur, ivec2 *next, dir dir) {
        switch (dir) {
        case UP:
                next->x = cur.x;
                next->y = cur.y - 1;
                if (isValidPos(*next)) {
                        return 0;
                } else {
                        next->x = -1;
                        next->y = -1;
                        return 1;
                }
        case RIGHT:
                next->x = cur.x + 1;
                next->y = cur.y;
                if (isValidPos(*next)) {
                        return 0;
                } else {
                        next->x = -1;
                        next->y = -1;
                        return 1;
                }
        case DOWN:
                next->x = cur.x;
                next->y = cur.y + 1;
                if (isValidPos(*next)) {
                        return 0;
                } else {
                        next->x = -1;
                        next->y = -1;
                        return 1;
                }
        case LEFT:
                next->x = cur.x - 1;
                next->y = cur.y;
                if (isValidPos(*next)) {
                        return 0;
                } else {
                        next->x = -1;
                        next->y = -1;
                        return 1;
                }
        default:
                return -1;
        }
}

int findTrails(node map[SIZE_Y][SIZE_X], ivec2 pos) {
        if (map[pos.y][pos.x].visit)
                return 0;

        map[pos.y][pos.x].visit = true;

        if (map[pos.y][pos.x].elev == 9) {
                return 1;
        }

        int trailCount = 0;
        int next = map[pos.y][pos.x].elev + 1;
        ivec2 nextpos = {-1, -1};

        if (getNextPos(pos, &nextpos, UP) == 0
                        && map[nextpos.y][nextpos.x].elev == next)
                trailCount += findTrails(map, nextpos);
        if (getNextPos(pos, &nextpos, RIGHT) == 0
                        && map[nextpos.y][nextpos.x].elev == next)
                trailCount += findTrails(map, nextpos);
        if (getNextPos(pos, &nextpos, DOWN) == 0
                        && map[nextpos.y][nextpos.x].elev == next)
                trailCount += findTrails(map, nextpos);
        if (getNextPos(pos, &nextpos, LEFT) == 0
                        && map[nextpos.y][nextpos.x].elev == next)
                trailCount += findTrails(map, nextpos);
        return trailCount;
}

int findTrails2(node map[SIZE_Y][SIZE_X], ivec2 pos) {
        if (map[pos.y][pos.x].elev == 9) {
                return 1;
        }

        int trailCount = 0;
        int next = map[pos.y][pos.x].elev + 1;
        ivec2 nextpos = {-1, -1};

        if (getNextPos(pos, &nextpos, UP) == 0
                        && map[nextpos.y][nextpos.x].elev == next)
                trailCount += findTrails2(map, nextpos);
        if (getNextPos(pos, &nextpos, RIGHT) == 0
                        && map[nextpos.y][nextpos.x].elev == next)
                trailCount += findTrails2(map, nextpos);
        if (getNextPos(pos, &nextpos, DOWN) == 0
                        && map[nextpos.y][nextpos.x].elev == next)
                trailCount += findTrails2(map, nextpos);
        if (getNextPos(pos, &nextpos, LEFT) == 0
                        && map[nextpos.y][nextpos.x].elev == next)
                trailCount += findTrails2(map, nextpos);
        return trailCount;
}

void part1(llist *ll) {
        SIZE_Y = ll->length;
        SIZE_X = getLongestLine(ll);
        node map[SIZE_Y][SIZE_X];

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < SIZE_X; x++) {
                        map[line][x].elev = str[x] - '0';
                        map[line][x].visit = false;
                }
                current = current->next;
                line++;
        }
        // printMap(map);

        int totalTrails = 0;
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        if (map[y][x].elev == 0) {
                                int trails = findTrails(map, (ivec2){x, y});
                                totalTrails += trails;
                                // printf("(%d, %d): %d\n", x, y, trails);
                                resetMap(map);
                        }
                }
        }

        printf("Part 1: Total Trails: %d\n\n", totalTrails);
}

void part2(llist *ll) {
        SIZE_Y = ll->length;
        SIZE_X = getLongestLine(ll);
        node map[SIZE_Y][SIZE_X];

        llNode *current = ll->head;
        int line = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < SIZE_X; x++) {
                        map[line][x].elev = str[x] - '0';
                        map[line][x].visit = false;
                }
                current = current->next;
                line++;
        }
        // printMap(map);

        int totalTrails = 0;
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        if (map[y][x].elev == 0) {
                                int trails = findTrails2(map, (ivec2){x, y});
                                totalTrails += trails;
                                // printf("(%d, %d): %d\n", x, y, trails);
                                resetMap(map);
                        }
                }
        }

        printf("Part 2: Total Trails: %d\n\n", totalTrails);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/test.txt");
        else
                ll = getInputFile("assets/inputs/2024/Day10.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
