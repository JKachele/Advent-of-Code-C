/*************************************************
 *File----------test
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 20, 2024 16:22:38 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include "src/util/util.h"

int MAX_X = 0;
int MAX_Y = 0;

// Finds the 2 nodes (nodeA, nodeB) from the 2 antenna Locations (a, b)
int getNode(vector2 a, vector2 b, vector2 *nodeA, vector2 *nodeB) {
        int diffX = a.x - b.x;
        int diffY = a.y - b.y;

        *nodeA = (vector2){a.x + diffX, a.y + diffY};
        *nodeB = (vector2){b.x - diffX, b.y - diffY};

        if (nodeA->x < 0 || nodeA->x >= MAX_X
                        || nodeA->y < 0 || nodeA->y >= MAX_Y) {
                *nodeA = (vector2){-1, -1};
        }
        if (nodeB->x < 0 || nodeB->x >= MAX_X
                        || nodeB->y < 0 || nodeB->y >= MAX_Y) {
                *nodeB = (vector2){-1, -1};
        }

        return 0;
}

int main(int argc, char *argv[]) {
        printf("Hello, World!\n");

        MAX_X = 10;
        MAX_Y = 10;
        vector2 nodeA = {0, 0};
        vector2 nodeB = {0, 0};

        getNode((vector2){5, 5}, (vector2){8, 4}, &nodeA, &nodeB);
        printf("[%d, %d]\n", nodeA.x, nodeA.y);
        printf("[%d, %d]\n", nodeB.x, nodeB.y);

        return 0;
}

