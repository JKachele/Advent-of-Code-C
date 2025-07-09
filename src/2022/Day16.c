/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 30, 2025 11:34:20 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

struct maxFlowData {
        int32 numV;
        int32 *distances;
        int32 *valveFlow;
        int32 timeLeft;
        bool *closedValves;
        int32 flow;
        int32 flowRate;
        int32 currentValve;
        int32 depth;
};

static int32 hashMap[26*26];
void initHashmap() {
        for (int i = 0; i < 26*26; i++) {
                hashMap[i] = -1;
        }
}
int32 getHashmapId(int32 index) {
        for (int i = 0; i < 26*26; i++) {
                if (hashMap[i] == index) {
                        return i;
                }
        }
        return -1;
}
void idToValve(int32 id, char *valve) {
        valve[0] = (id / 26) + 'A';
        valve[1] = (id % 26) + 'A';
        valve[2] = '\0';
}
void printValve(int32 id) {
        char valve[3];
        valve[0] = (id / 26) + 'A';
        valve[1] = (id % 26) + 'A';
        valve[2] = '\0';
        printf("%s ", valve);
}

int32 valve2ID(char *valve) {
        int32 id = (valve[0] - 'A') * 26;
        id += valve[1] - 'A';
        return id;
}


void printPath(int32 num, int32 path[], int flow) {
        printf("%d: ", flow);
        for (int i = 0; i < num; i++) {
                if (path[i] == -1)
                        break;
                printValve(getHashmapId(path[i]));
        }
        printf("\n");
}

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printDists(int32 num, int32 dists[num][num]) {
        if (!Debug) return;
        printf("   ");
        for (int i = 0; i < num; i++) {
                char valve[3];
                idToValve(i, valve);
                printf("%s ", valve);
        }
        printf("\n");
        for (int i = 0; i < num; i++) {
                char valve[3];
                idToValve(i, valve);
                printf("%s ", valve);
                for (int j = 0; j < num; j++) {
                        int32 n = dists[i][j];
                        if (n != 1000000)
                                printf("%d  ", dists[i][j]);
                        else
                                printf("*  ");
                }
                printf("\n");
        }
        printf("\n");
}

void floydWarshallAlg(int32 num, int32 dists[num][num]) {
        MAKE_LOOP(p, num, i, num, j, num) {
                int32 pivDist = dists[i][p] + dists[p][j];
                if (pivDist < 0) pivDist = INT32_MAX;
                if (dists[i][j] > pivDist)
                        dists[i][j] = pivDist;
        }
}

int32 findMaxFlow(struct maxFlowData *data, int32 path[]) {
        // Expand Variables
        int32 (*distances)[data->numV] = (int32 (*)[data->numV])data->distances;

        // Return total flow if no time is left or all valves are open
        if (data->timeLeft <= 0) {
                return data->flow;
        }
        bool allValvesOpen = true;
        for (int i = 0; i < data->numV; i++) {
                if (data->closedValves[i]) {
                        allValvesOpen = false;
                        break;
                }
        }
        if (allValvesOpen) {
                return data->flow + (data->flowRate * data->timeLeft);
        }

        int32 finalValves[data->numV];
        // Loop through remaining closed valves and try each
        int32 maxFlow = 0;
        for (int i = 0; i < data->numV; i++) {
                // Skip valve of already open
                if (!data->closedValves[i]) continue;

                struct maxFlowData newData = {0};
                newData.numV = data->numV;
                newData.distances = data->distances;
                newData.valveFlow = data->valveFlow;

                // Copy closedValves Array
                bool closedValvesCp[data->numV];
                memcpy(closedValvesCp, data->closedValves, data->numV * sizeof(bool));
                newData.closedValves = (bool*)closedValvesCp;

                int32 timeToOpen = distances[data->currentValve][i] + 1;
                newData.flow = data->flow + (timeToOpen * data->flowRate);
                newData.timeLeft = data->timeLeft - timeToOpen;
                newData.flowRate = data->flowRate + data->valveFlow[i];
                newData.closedValves[i] = false;
                newData.currentValve = i;
                newData.depth = data->depth + 1;

                int32 totalFlow;
                if (newData.timeLeft >= 0) {
                        path[data->depth] = i;
                        totalFlow = findMaxFlow(&newData, path);
                } else {
                        totalFlow = data->flow + (data->timeLeft * data->flowRate); 
                        // printPath(data->numV, path, totalFlow);
                }

                if (totalFlow > maxFlow) {
                        maxFlow = totalFlow;
                }
                path[data->depth] = -1;
        }
        return maxFlow;
}

void part1(llist *ll) {
        initHashmap();
        struct maxFlowData data;
        data.numV = ll->length;
        int32 distances[data.numV][data.numV];
        int32 flow[data.numV];
        data.currentValve = 0;
        MAKE_LOOP(i, data.numV, j, data.numV)
                distances[i][j] = INT32_MAX;

        llNode *current = ll->head;
        int32 valvesSeen = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                // get valve id and index
                strtok(str, " ");
                char *valve = strtok(NULL, " ");
                int32 id = valve2ID(valve);
                int32 index = valvesSeen;
                if (hashMap[id] == -1) {
                        hashMap[id] = valvesSeen;
                        valvesSeen++;
                } else {
                        index = hashMap[id];
                }
                // printf("%s: %d\n", valve, index);
                distances[index][index] = 0;

                // Get starting Valve
                if (strcmp("AA", valve) == 0) {
                        printf("Start at %d\n", index);
                        data.currentValve = index;
                }

                // Get Valve flow rate
                strtok(NULL, "=");
                char *flowStr = strtok(NULL, ";");
                flow[index] = strtol(flowStr, (char**)NULL, 10);

                // Get valve neighbors
                for (int i = 0; i < 4; i++)
                        strtok(NULL, " ");
                char *next = strtok(NULL, ",");
                while (next != NULL) {
                        int32 nextId = valve2ID(next);
                        int32 nextIndex = valvesSeen;
                        if (hashMap[nextId] == -1) {
                                hashMap[nextId] = valvesSeen;
                                valvesSeen++;
                        } else {
                                nextIndex = hashMap[nextId];
                        }
                        distances[index][nextIndex] = 1;
                        distances[nextIndex][index] = 1;

                        next = strtok(NULL, " ");
                        if (next != NULL) 
                                next = strtok(NULL, ",");
                }

                current = current->next;
        }

        floydWarshallAlg(data.numV, distances);
        // printDists(data.numV, distances);
        bool closedValves[data.numV];
        for (int i = 0; i < data.numV; i++) {
                if (flow[i] > 0) {
                        closedValves[i] = true;
                } else {
                        closedValves[i] = false;
                }
        }

        data.distances = (int32*)distances;
        data.valveFlow = (int32*)flow;
        data.timeLeft = 30;
        data.closedValves = (bool*)closedValves;
        data.flow = 0;
        data.flowRate = 0;
        data.depth = 0;

        int32 path[data.numV];
        for (int i = 0; i < data.numV; i++)
                path[i] = -1;

        int32 maxFlow = findMaxFlow(&data, path);

        printf("Part 1: Max Flow: %d\n\n", maxFlow);
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
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                // ll = getInputFile("assets/tests/2022/Day16.txt");
                // ll = getInputFile("assets/tests/2022/Day16b.txt");
                ll = getInputFile("assets/tests/2022/Day16c.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day16.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

