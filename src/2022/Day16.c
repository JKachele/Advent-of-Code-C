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

static int hashMap[26*26];
void initHashmap() {
        for (int i = 0; i < 26*26; i++) {
                hashMap[i] = -1;
        }
}
int getHashmapId(int index) {
        for (int i = 0; i < 26*26; i++) {
                if (hashMap[i] == index) {
                        return i;
                }
        }
        return -1;
}
void idToValve(int id, char *valve) {
        valve[0] = (id / 26) + 'A';
        valve[1] = (id % 26) + 'A';
        valve[2] = '\0';
}
void printValve(int id) {
        char valve[3];
        valve[0] = (id / 26) + 'A';
        valve[1] = (id % 26) + 'A';
        valve[2] = '\0';
        printf("%s ", valve);
}

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printDists(int num, int dists[num][num]) {
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
                        int n = dists[i][j];
                        if (n != 1000000)
                                printf("%d  ", dists[i][j]);
                        else
                                printf("*  ");
                }
                printf("\n");
        }
        printf("\n");
}

int valve2ID(char *valve) {
        int id = (valve[0] - 'A') * 26;
        id += valve[1] - 'A';
        return id;
}

void floydWarshallAlg(int num, int dists[num][num]) {
        MAKE_LOOP(p, num, i, num, j, num) {
                int pivDist = dists[i][p] + dists[p][j];
                if (dists[i][j] > pivDist)
                        dists[i][j] = pivDist;
        }
}

int findMaxFlow(int numValves, int distances[][numValves], int valveFlow[],
                int timeLeft, bool closedValves[], int flow, int flowRate,
                int currentValve, int depth, int valvesOpened[]) {
        // Return total flow if no time is left or all valves are open
        if (timeLeft <= 0) {
                return flow;
        }
        bool allValvesOpen = true;
        for (int i = 0; i < numValves; i++) {
                if (closedValves[i]) {
                        allValvesOpen = false;
                        break;
                }
        }
        if (allValvesOpen) {
                return flow + (flowRate * timeLeft);
        }

        int finalValves[numValves];
        // Loop through remaining closed valves and try each
        int maxFlow = 0;
        for (int i = 0; i < numValves; i++) {
                // Skip valve of already open
                if (!closedValves[i]) continue;

                // Copy closedValves Array
                bool closedValvesCp[numValves];
                memcpy(closedValvesCp, closedValves, numValves * sizeof(bool));

                int timeToOpen = distances[currentValve][i] + 1;
                int newFlow = flow + (timeToOpen * flowRate);
                int newTimeLeft = timeLeft - timeToOpen;
                int newFlowRate = flowRate + valveFlow[i];
                closedValvesCp[i] = false;
                valvesOpened[depth] = i;

                int totalFlow;
                if (newTimeLeft >= 0) {
                        totalFlow = findMaxFlow(numValves, distances, valveFlow,
                                newTimeLeft, closedValvesCp, newFlow,
                                newFlowRate, i, depth + 1, valvesOpened);
                } else {
                        totalFlow = flow + (timeLeft * flowRate); 
                }

                if (totalFlow > maxFlow) {
                        maxFlow = totalFlow;
                        memcpy(finalValves, valvesOpened,
                                        numValves * sizeof(int));
                }
        }
        memcpy(valvesOpened, finalValves, numValves * sizeof(int));
        return maxFlow;
}

void part1(llist *ll) {
        initHashmap();
        int numValves = ll->length;
        int distances[numValves][numValves];
        MAKE_LOOP(i, numValves, j, numValves)
                distances[i][j] = 1000000;
        int flow[numValves];
        int startValve = 0;

        llNode *current = ll->head;
        int valvesSeen = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                // get valve id and index
                strtok(str, " ");
                char *valve = strtok(NULL, " ");
                int id = valve2ID(valve);
                int index = valvesSeen;
                if (hashMap[id] == -1) {
                        hashMap[id] = valvesSeen;
                        valvesSeen++;
                } else {
                        index = hashMap[id];
                }
                distances[index][index] = 0;

                // Get starting Valve
                if (strcmp("AA", valve) == 0) {
                        printf("Start at %d\n", index);
                        startValve = index;
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
                        int nextId = valve2ID(next);
                        int nextIndex = valvesSeen;
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

        printf("Start flow: %d\n", flow[startValve]);

        floydWarshallAlg(numValves, distances);
        // printDists(numValves, distances);
        bool closedValves[numValves];
        int valvesOpened[numValves];
        for (int i = 0; i < numValves; i++) {
                valvesOpened[i] = -1;
                if (flow[i] > 0) {
                        closedValves[i] = true;
                } else {
                        closedValves[i] = false;
                }
        }
        int maxFlow = findMaxFlow(numValves, distances, flow, 30, 
                closedValves, 0, 0, startValve, 0, valvesOpened);

        for (int i = 0; i < numValves; i++) {
                printf("%d ", hashMap[valvesOpened[i]]);
        }
        printf("\n");
        for (int i = 0; i < numValves; i++) {
                char val[5];
                idToValve(hashMap[valvesOpened[i]], val);
                printf("%s ", val);
        }
        printf("\n");


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
                ll = getInputFile("assets/tests/2022/Day16b.txt");
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

