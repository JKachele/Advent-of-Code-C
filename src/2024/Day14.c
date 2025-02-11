/*************************************************
 *File----------Day14.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Jan 10, 2025 09:16:52 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <limits.h>
#include <stdio.h>
#include <string.h>
// #include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"

#define SIZE_X 101
#define SIZE_Y 103

#define NUM_STEPS 100

void clearFile(char *fileName) {
        FILE *outFile = fopen(fileName, "w");
        fclose(outFile);
}

void mapToFile(char* fileName, ivec2 guards[], int numGuards, int step) {
        if (step == 0)
                clearFile(fileName);

        FILE *outFile = fopen(fileName, "a");
        if (outFile == NULL) {    // If file dosent exist, exit
                exit(EXIT_FAILURE);
        }

        int grid[SIZE_Y][SIZE_X] = {0};
        for (int i = 0; i < numGuards; i++){
                // printf("%d, %d\n", guards[i].x, guards[i].y);
                grid[guards[i].y][guards[i].x]++;
        }

        fprintf(outFile, "Time = %d Seconds\n", step);
        for (int y = 0; y < SIZE_Y; y++) {
                for (int x = 0; x < SIZE_X; x++) {
                        if (grid[y][x] == 0)
                                fprintf(outFile, ".");
                        else
                                fprintf(outFile, "%d", grid[y][x]);
                }
                fprintf(outFile, "\n");
        }

        fclose(outFile);
}

void stepGuards(ivec2 guards[], ivec2 guardVel[], int n, int steps) {
        for(int i = 0; i < n; i++) {
                ivec2 p = guards[i];
                ivec2 v = guardVel[i];
                ivec2 newP;
                newP.x = (p.x + (steps * v.x)) % SIZE_X;
                newP.y = (p.y + (steps * v.y)) % SIZE_Y;
                if (newP.x < 0)
                        newP.x = SIZE_X + newP.x;
                if (newP.y < 0)
                        newP.y = SIZE_Y + newP.y;
                guards[i].x = newP.x;
                guards[i].y = newP.y;
        }
}

// 1 2
// 3 4  (0 for no quadrant, -1 for error)
int quadrant(ivec2 p) {
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

long getSafteyFactor(ivec2 guards[], int numGuards) {
        int quad1Count = 0;
        int quad2Count = 0;
        int quad3Count = 0;
        int quad4Count = 0;

        for (int i = 0; i < numGuards; i++) {
                int quad = quadrant(guards[i]);
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
                        printf("Error with guard starting at (%d, %d)",
                                        guards[i].x, guards[i].y);
                        break;
                }
        }

        long safetyFactor = quad1Count * quad2Count * quad3Count * quad4Count;
        return safetyFactor;
}

int getMaxOverlap(ivec2 guards[], int numGuards) {
        int grid[SIZE_Y][SIZE_X] = {0};
        int maxOverlap = 0;
        for (int i = 0; i < numGuards; i++){
                grid[guards[i].y][guards[i].x]++;
                if (grid[guards[i].y][guards[i].x] > maxOverlap)
                        maxOverlap = grid[guards[i].y][guards[i].x];
        }
        return maxOverlap;
}

void part1(llist *ll) {
        int quad1Count = 0;
        int quad2Count = 0;
        int quad3Count = 0;
        int quad4Count = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                ivec2 p;
                ivec2 v;
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

                ivec2 newP;
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
                        printf("Error with guard starting at (%d, %d)",
                                        p.x, p.y);
                        break;
                }

                // printf("P: (%d, %d), V: (%d, %d) newP: (%d, %d) Quad: %d\n",
                //                 p.x, p.y, v.x, v.y, newP.x, newP.y, quad);

                current = current->next;
        }

        printf("Quads: 1:%d, 2:%d, 3:%d, 4:%d\n",
                        quad1Count, quad2Count, quad3Count, quad4Count);
        long safetyFactor = quad1Count * quad2Count * quad3Count * quad4Count;

        printf("Part 1: Safety Factor: %ld\n\n", safetyFactor);
}

void part2(llist *ll) {
        ivec2 guards[ll->length];
        ivec2 guardVel[ll->length];

        llNode *current = ll->head;
        int guardNum = 0;
        while(current != NULL) {
                ivec2 p;
                ivec2 v;
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

                guards[guardNum] = p;
                guardVel[guardNum] = v;

                current = current->next;
                guardNum++;
        }

        char *file1 = "assets/misc/2024/Day14a.txt";
        for (int i = 0; i < NUM_STEPS; i++) {
                mapToFile(file1, guards, ll->length, i);
                stepGuards(guards, guardVel, ll->length, 1);
        }
        stepGuards(guards, guardVel, ll->length, -100);

        char *file2 = "assets/misc/2024/Day14b.txt";
        mapToFile(file2, guards, ll->length, 0);
        stepGuards(guards, guardVel, ll->length, 79);
        for (int i = 79; i < SIZE_X * 100; i += SIZE_X) {
                mapToFile(file2, guards, ll->length, i);
                stepGuards(guards, guardVel, ll->length, SIZE_X);
        }

        // printf("Part 2: \n");
}

// Find Part 2 answer programatically
void part2a(llist *ll) {
        ivec2 guards[ll->length];
        ivec2 guardVel[ll->length];

        llNode *current = ll->head;
        int guardNum = 0;
        while(current != NULL) {
                ivec2 p;
                ivec2 v;
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

                guards[guardNum] = p;
                guardVel[guardNum] = v;

                current = current->next;
                guardNum++;
        }

        // Get step with the lowest safety factor for starting point
        long minSafetyFactor = LONG_MAX;
        int minIndex = 0;
        for (int i = 0; i < NUM_STEPS; i++) {
                long safteyFactor = getSafteyFactor(guards, ll->length);
                if (safteyFactor < minSafetyFactor) {
                        minSafetyFactor = safteyFactor;
                        minIndex = i;
                }
                stepGuards(guards, guardVel, ll->length, 1);
        }
        stepGuards(guards, guardVel, ll->length, -NUM_STEPS); // Reset Guards

        stepGuards(guards, guardVel, ll->length, minIndex);
        int treeIndex = minIndex;
        while (getMaxOverlap(guards, ll->length) != 1) {
                stepGuards(guards, guardVel, ll->length, SIZE_X);
                treeIndex += SIZE_X;
        }
        

        printf("Part 2: Min Tree Index: %d\n", treeIndex);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day14.txt");
        else
                ll = getInputFile("assets/2024/Day14.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);
        part2a(ll);

        return 0;
}
