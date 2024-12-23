/*************************************************
 *File----------Day8.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Dec 19, 2024 15:59:52 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"

int MAX_X = 0;
int MAX_Y = 0;

typedef struct {
        char item;
        bool node;
} cell;

typedef struct {
        char freq;
        vector2 location;
} antenna;

void printAntenna(void *data) {
        if (data == NULL)
                return;
        antenna *a = (antenna*)data;
        printf("%c:(%d, %d)", a->freq, a->location.x, a->location.y);
}

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

int getFreqNodes(cell grid[MAX_Y][MAX_X], llist *antennas) {
        vector2 nodeA = {-1, -1};
        vector2 nodeB = {-1, -1};
        for (int i = 0; i < antennas->length; i++) {
                llNode *llNodeA = llist_get_node(antennas, i);
                antenna *antennaA = (antenna*)(llNodeA->data);
                vector2 a = antennaA->location;
                for (int j = i + 1; j < antennas->length; j++) {
                        llNode *llNodeB = llist_get_node(antennas, j);
                        antenna *antennaB = (antenna*)(llNodeB->data);
                        vector2 b = antennaB->location;
                        getNode(a, b, &nodeA, &nodeB);
                        if (nodeA.x != -1) {
                                grid[nodeA.y][nodeA.x].node = true;
                        }
                        if (nodeB.x != -1) {
                                grid[nodeB.y][nodeB.x].node = true;
                        }
                }
        }
        return 0;
}

void part1(llist *ll) {
        MAX_X = strlen((char*)ll->head->data);
        MAX_Y = ll->length;
        llist *antennas = llist_create();
        bool freqsPresent[127] = {false};
        int numFreqs = 0;

        cell grid[MAX_Y][MAX_X];
        for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        grid[y][x].node = false;
                        grid[y][x].item = '.';
                }
        }

        llNode *current = ll->head;
        int lineNum = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < MAX_X; x++) {
                        grid[lineNum][x].item = str[x];
                        if (str[x] != '.') {
                                if (freqsPresent[str[x]] == 0)
                                        numFreqs++;
                                freqsPresent[str[x]] = true;

                                antenna *curAntenna = malloc(sizeof(antenna));
                                curAntenna->freq = str[x];
                                curAntenna->location = (vector2){x, lineNum};
                                llist_add(antennas, curAntenna);
                        }
                }
                current = current->next;
                lineNum++;
        }
        /* for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        printf("%c", grid[y][x].item);
                }
                printf("\n");
        }
        llist_print(antennas, printAntenna); */

        char freqs[numFreqs];
        int counter = 0;
        for (int i = 0; i < 127; i++) {
                if (freqsPresent[i]) {
                        freqs[counter] = i;
                        counter++;
                }
        }

        llist* freqAntennas[numFreqs];
        for (int i = 0; i < numFreqs; i++) {
                freqAntennas[i] = llist_create();
                llNode *curAntennaNode = antennas->head;
                while (curAntennaNode != NULL) {
                        antenna *curAntenna = malloc(sizeof(antenna));
                        *curAntenna = *((antenna*)curAntennaNode->data);
                        if(curAntenna->freq == freqs[i])
                                llist_add(freqAntennas[i], curAntenna);
                        curAntennaNode = curAntennaNode->next;
                }
        }
        llist_free(antennas);

        for (int i = 0; i < numFreqs; i++) {
                getFreqNodes(grid, freqAntennas[i]);
                llist_free(freqAntennas[i]);
        }

        int numNodes = 0;
        for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        if (grid[y][x].node)
                                numNodes++;
                }
        }

        printf("Part 1: Unique Nodes: %d\n\n", numNodes);
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
        llist *ll = getInputFile("assets/2024/Day8.txt");
        // llist *ll = getInputFile("assets/test.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
