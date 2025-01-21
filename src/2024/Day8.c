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
        ivec2 location;
} antenna;

void printAntenna(void *data) {
        if (data == NULL)
                return;
        antenna *a = (antenna*)data;
        printf("%c:(%d, %d)", a->freq, a->location.x, a->location.y);
}

// Euclid's Algorithm
int gcd(int a, int b) {
        int temp;
        while (b != 0) {
                temp = a % b;
                a = b;
                b = temp;
        }
        return a;
}

bool isValidPos(ivec2 p) {
        return (p.x >= 0 && p.y >= 0 && p.x < MAX_X && p.y < MAX_Y);
}

// Finds the 2 nodes (nodeA, nodeB) from the 2 antenna Locations (a, b)
int getNode(ivec2 a, ivec2 b, ivec2 *nodeA, ivec2 *nodeB) {
        int diffX = b.x - a.x;
        int diffY = b.y - a.y;

        *nodeA = (ivec2){a.x - diffX, a.y - diffY};
        *nodeB = (ivec2){b.x + diffX, b.y + diffY};

        if (nodeA->x < 0 || nodeA->x >= MAX_X
                        || nodeA->y < 0 || nodeA->y >= MAX_Y) {
                *nodeA = (ivec2){-1, -1};
        }
        if (nodeB->x < 0 || nodeB->x >= MAX_X
                        || nodeB->y < 0 || nodeB->y >= MAX_Y) {
                *nodeB = (ivec2){-1, -1};
        }

        return 0;
}

int getFreqNodes(cell grid[MAX_Y][MAX_X], llist *antennas) {
        ivec2 nodeA = {-1, -1};
        ivec2 nodeB = {-1, -1};
        for (int i = 0; i < antennas->length; i++) {
                llNode *llNodeA = llist_get_node(antennas, i);
                antenna *antennaA = (antenna*)(llNodeA->data);
                ivec2 a = antennaA->location;
                for (int j = i + 1; j < antennas->length; j++) {
                        llNode *llNodeB = llist_get_node(antennas, j);
                        antenna *antennaB = (antenna*)(llNodeB->data);
                        ivec2 b = antennaB->location;
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

int getNodes2(cell grid[MAX_Y][MAX_X], ivec2 a, ivec2 b) {
        ivec2 slope = {b.x - a.x, b.y - a.y};

        // Verify no nodes between antennas
        int slopeGCD = gcd(slope.x, slope.y);
        if ( slopeGCD > 1) {
                fprintf(stderr, "ERROR: GCD=%d, need to check "
                                "non-integer multiples", slopeGCD);
                return 1;
        }

        // Subtract Slope
        ivec2 curPos = a;
        while (isValidPos(curPos)) {
                grid[curPos.y][curPos.x].node = true;
                curPos.x -= slope.x;
                curPos.y -= slope.y;
        }
        // Add Slope
        curPos = a;
        while (isValidPos(curPos)) {
                grid[curPos.y][curPos.x].node = true;
                curPos.x += slope.x;
                curPos.y += slope.y;
        }

        return 0;
}

int getFreqNodes2(cell grid[MAX_Y][MAX_X], llist *antennas) {
        for (int i = 0; i < antennas->length; i++) {
                llNode *llNodeA = llist_get_node(antennas, i);
                antenna *antennaA = (antenna*)(llNodeA->data);
                ivec2 a = antennaA->location;
                for (int j = i + 1; j < antennas->length; j++) {
                        llNode *llNodeB = llist_get_node(antennas, j);
                        antenna *antennaB = (antenna*)(llNodeB->data);
                        ivec2 b = antennaB->location;

                        getNodes2(grid, a, b);
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
                        if (str[x] == '.')
                                continue;

                        if (freqsPresent[str[x]] == 0)
                                numFreqs++;
                        freqsPresent[str[x]] = true;

                        antenna *curAntenna = malloc(sizeof(antenna));
                        curAntenna->freq = str[x];
                        curAntenna->location = (ivec2){x, lineNum};
                        llist_add(antennas, curAntenna);
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

        for (int i = 0; i < numFreqs; i++) {
                llist *freqAntennas = llist_create();
                llNode *curAntennaNode = antennas->head;
                while (curAntennaNode != NULL) {
                        antenna *curAntenna = malloc(sizeof(antenna));
                        *curAntenna = *((antenna*)curAntennaNode->data);
                        if(curAntenna->freq == freqs[i])
                                llist_add(freqAntennas, curAntenna);
                        curAntennaNode = curAntennaNode->next;
                }
                getFreqNodes(grid, freqAntennas);
                llist_free(freqAntennas);
        }
        llist_free(antennas);

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
        MAX_X = strlen((char*)ll->head->data);
        MAX_Y = ll->length;
        cell grid[MAX_Y][MAX_X];
        llist *antennas = llist_create();
        bool freqsPresent[127] = {false};
        int numFreqs = 0;


        llNode *current = ll->head;
        int lineNum = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                for (int x = 0; x < MAX_X; x++) {
                        grid[lineNum][x].item = str[x];
                        grid[lineNum][x].node = false;
                        if (str[x] == '.')
                                continue;

                        if (!freqsPresent[str[x]])
                                numFreqs++;
                        freqsPresent[str[x]] = true;

                        antenna *curAntenna = malloc(sizeof(antenna));
                        curAntenna->freq = str[x];
                        curAntenna->location = (ivec2){x, lineNum};
                        llist_add(antennas, curAntenna);
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

        for (int i = 0; i < numFreqs; i++) {
                llist *freqAntennas = llist_create();
                llNode *curAntennaNode = antennas->head;
                while (curAntennaNode != NULL) {
                        antenna *curAntenna = malloc(sizeof(antenna));
                        *curAntenna = *((antenna*)curAntennaNode->data);
                        if(curAntenna->freq == freqs[i])
                                llist_add(freqAntennas, curAntenna);
                        curAntennaNode = curAntennaNode->next;
                }
                getFreqNodes2(grid, freqAntennas);
                llist_free(freqAntennas);
        }
        llist_free(antennas);

        int numNodes = 0;
        for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                        if (grid[y][x].node)
                                numNodes++;
                }
        }

        printf("Part 2: Unique Nodes: %d\n\n", numNodes);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/test.txt");
        else
                ll = getInputFile("assets/2024/Day8.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
