/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Jan 10, 2025 09:16:52 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
// #include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"

#define SIZE_X 101
#define SIZE_Y 103

#define NUM_STEPS 100

// 1 2
// 3 4  (0 for no quadrant, -1 for error)
int quadrant(vector2 p) {
        int qSizeX = (SIZE_X - 1) / 2;
        int qSizeY = (SIZE_Y - 1) / 2;

        // if pos is in the center, no quadrant
        if (p.x == qSizeX || p.y == qSizeY)
                return 0;

        if (p.x < qSizeX && p.y < qSizeY)
                return 1;
        if (p.x > qSizeX && p.y < qSizeY)
                return 2;
        if (p.x < qSizeX && p.y > qSizeY)
                return 3;
        if (p.x > qSizeX && p.y > qSizeY)
                return 4;

        return -1;
}

void part1(llist *ll) {
        int quad1Count = 0;
        int quad2Count = 0;
        int quad3Count = 0;
        int quad4Count = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                vector2 p;
                vector2 v;
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                strtok(str, "=");
                char *px = strtok(NULL, ",");
                p.x = strtol(px, (char**)NULL, 10);
                char *py = strtok(NULL, " ");
                p.y = strtol(py, (char**)NULL, 10);
                strtok(NULL, "=");
                char *vx = strtok(NULL, ",");
                v.x = strtol(vx, (char**)NULL, 10);
                char *vy = strtok(NULL, "");
                v.y = strtol(vy, (char**)NULL, 10);

                vector2 newP;
                newP.x = (p.x + (NUM_STEPS * v.x)) % SIZE_X;
                newP.y = (p.y + (NUM_STEPS * v.y)) % SIZE_Y;
                if (newP.x < 0)
                        newP.x = SIZE_X + newP.x;
                if (newP.y < 0)
                        newP.y = SIZE_Y + newP.y;

                int quad = quadrant(newP);
                switch (quad) {
                case 0:
                        break;
                case 1:
                        quad1Count ++;
                        break;
                case 2:
                        quad2Count ++;
                        break;
                case 3:
                        quad3Count ++;
                        break;
                case 4:
                        quad4Count ++;
                        break;
                default:
                        printf("Error with guard starting at (%ld, %ld)",
                                        p.x, p.y);
                        break;
                }

                // printf("P: (%ld, %ld), V: (%ld, %ld) newP: (%ld, %ld) Quad: %d\n",
                //                 p.x, p.y, v.x, v.y, newP.x, newP.y, quad);

                current = current->next;
        }

        printf("Quads: 1:%d, 2:%d, 3:%d, 4:%d\n",
                        quad1Count, quad2Count, quad3Count, quad4Count);
        long safetyFactor = quad1Count * quad2Count * quad3Count * quad4Count;

        printf("Part 1: Safety Factor: %ld\n\n", safetyFactor);
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
        llist *ll = getInputFile("assets/2024/Day14.txt");
        // llist *ll = getInputFile("assets/test.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
