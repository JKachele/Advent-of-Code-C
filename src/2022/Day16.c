/*************************************************
 *File----------Day16.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Jul 10, 2025 13:36:38 UTC
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

#define TIME_LIMIT 30

typedef tll(int32) tllint;

struct valve {
        char name[3];
        int32 id;
        int32 index;
        int32 flow;
        tllint neighbors;
};

typedef tll(struct valve) tllvalve;

struct state {
        tllint opened;
        int32 cur;
        int32 elapsed;
        int32 relieved;
};

typedef tll(struct state) tllstate;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int32 valve2ID(char *valve) {
        int32 id = (valve[0] - 'A') * 26;
        id += valve[1] - 'A';
        return id;
}

void idToValve(int32 id, char *valve) {
        valve[0] = (id / 26) + 'A';
        valve[1] = (id % 26) + 'A';
        valve[2] = '\0';
}

void printValves(tllvalve valves) {
        tll_foreach(valves, it) {
                struct valve valve = it->item;
                printf("%s(%d): flow: %d neighbors: ",
                                valve.name, valve.id, valve.flow);
                tll_foreach(valve.neighbors, vit) {
                        char nvalve[3];
                        idToValve(vit->item, nvalve);
                        printf("%s ", nvalve);
                }
                printf("\n");
        }
}

int getIndex(tllvalve valves, int32 id) {
        tll_foreach(valves, it) {
                if (it->item.id == id)
                        return it->item.index;
        }
        return -1;
}

void getDistances(int32 numV, tllvalve valves, int32 dists[numV][numV]) {
        // Initaloze array
        MAKE_LOOP(i, numV, j, numV)
                dists[i][j] = INT32_MAX;

        // Set neighbors to 1
        tll_foreach(valves, it) {
                struct valve valve = it->item;
                int vIndex = valve.index;
                dists[vIndex][vIndex] = 0;
                tll_foreach(valve.neighbors, vit) {
                        int nIndex = getIndex(valves, vit->item);
                        dists[vIndex][nIndex] = 1;
                        dists[nIndex][vIndex] = 1;
                }
        }

        MAKE_LOOP(p, numV, i, numV, j, numV) {
                int32 pivDist = dists[i][p] + dists[p][j];
                if (pivDist < 0) pivDist = INT32_MAX;
                if (dists[i][j] > pivDist)
                        dists[i][j] = pivDist;
        }
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

int32 getNumFlowing(tllvalve valves) {
        int32 flowing = 0;
        tll_foreach(valves, it) {
                if (it->item.flow > 0)
                        flowing++;
        }
        return flowing;
}

void getUnopened(tllvalve valves, struct state state, tllint *unopened) {
                tll_foreach(valves, it) {
                        int32 id = it->item.id;
                        bool opened = false;
                        tll_foreach(state.opened, oit) {
                                if (oit->item == id) {
                                        opened = true;
                                        break;
                                }
                        }
                        if (!opened)
                                tll_push_back(*unopened, id);
                }
}

int32 waitTillEnd(tllvalve valves, struct state state) {
        int32 timeLeft = TIME_LIMIT - state.elapsed;
        int32 flowRate = 0;
        tll_foreach(state.opened, it) {
                int id = it->item;
                tll_foreach(valves, vit) {
                        if (vit->item.id == id)
                                flowRate += vit->item.flow;
                }
        }
        return state.relieved + (flowRate * timeLeft);
}

int32 getMaxFlow(int32 numV, tllvalve valves, int32 distances[numV][numV]) {
        tllstate stateQueue = tll_init();
        int32 numFlowing = getNumFlowing(valves);

        // Add inital state to the queue
        struct state initState;
        initState.opened = (tllint)tll_init();
        initState.cur = valve2ID("AA");
        initState.elapsed = 0;
        initState.relieved = 0;
        tll_push_back(stateQueue, initState);

        int32 max_relieved = 0;
        while (tll_length(stateQueue) > 0) {
                struct state curState = tll_pop_front(stateQueue);

                // if all flowing valves are open, wait until end
                if (tll_length(curState.opened) == numFlowing) {
                        int32 totalRelieved = waitTillEnd(valves, curState);
                        if (totalRelieved > max_relieved)
                                max_relieved = totalRelieved;
                        continue;
                }

                // Get list of unopened valves
                tllint unpoened = tll_init();
                getUnopened(valves, curState, &unpoened);

                tll_foreach(unpoened, it) {
                        struct valve valve;
                        tll_foreach(valves, vit) {
                                if (vit->item.id == it->item) {
                                        valve = vit->item;
                                        break;
                                }
                        }

                        int32 dist;
                }
        }

        return 0;
}

void part1(llist *ll) {
        tllvalve valves = tll_init();

        llNode *current = ll->head;
        int index = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                struct valve curValve = {0};

                // get valve id
                strtok(str, " ");
                char *valve = strtok(NULL, " ");
                strncpy(curValve.name, valve, 2);
                curValve.id = valve2ID(valve);
                curValve.index = index;
                index++;

                // Get Valve flow rate
                strtok(NULL, "=");
                char *flowStr = strtok(NULL, ";");
                curValve.flow = strtol(flowStr, (char**)NULL, 10);

                // Get valve neighbors
                curValve.neighbors = (tllint)tll_init();
                for (int i = 0; i < 4; i++)
                        strtok(NULL, " ");
                char *next = strtok(NULL, ",");
                while (next != NULL) {
                        int32 nextId = valve2ID(next);
                        tll_push_back(curValve.neighbors, nextId);
                        next = strtok(NULL, " ");
                        if (next != NULL) 
                                next = strtok(NULL, ",");
                }

                tll_push_back(valves, curValve);

                current = current->next;
        }
        // printValves(valves);
        int32 numV = tll_length(valves);


        int32 distances[numV][numV];
        getDistances(numV, valves, distances);
        printDists(numV, distances);

        int32 maxFlow = getMaxFlow(numV, valves, distances);

        printf("Part 1: \n\n");
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
                ll = getInputFile("assets/tests/2022/Day16.txt");
                // ll = getInputFile("assets/tests/2022/Day16b.txt");
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

