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
#define TIME_LIMIT2 26

typedef tll(int32) tllint;

struct valve {
        char name[3];
        int32 id;
        int32 index;
        int32 flow;
        tllint neighbors;
};

struct state {
        tllint opened;
        tllint unopened;
        int32 cur;
        int32 cur2;
        int32 elapsed;
        int32 flowRate;
        int32 relieved;
};

typedef tll(struct state) tllstate;

struct state2 {
        tllint opened;
        tllint unopened;
        int32 cur1;
        int32 cur2;
        int32 elapsed;
        int32 flowRate;
        int32 relieved;
};

typedef tll(struct state2) tllstate2;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printTllint(tllint tllist) {
        printf("[ ");
        tll_foreach(tllist, it) {
                printf("%d ", it->item);
        }
        printf("]");
}

void printState(struct state state, bool oneLine) {
        if (!Debug) return;
        if (oneLine) {
                printTllint(state.opened);
                printf(", ");
                printTllint(state.unopened);
                printf(", %d", state.cur);
                printf(", %d", state.elapsed);
                printf(", %d", state.flowRate);
                printf(", %d\n", state.relieved);
                return;
        }
        printf("{\n\t");
        printTllint(state.opened);
        printf("\n\t");
        printTllint(state.unopened);
        printf("\n\tcur: %d\n", state.cur);
        printf("\telapsed: %d\n", state.elapsed);
        printf("\tflowRate: %d\n", state.flowRate);
        printf("\trelieved: %d\n", state.relieved);
        printf("}\n");
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

void printValves(int32 numV, struct valve valves[]) {
        for (int i = 0; i < numV; i++) {
                struct valve valve = valves[i];
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

void tllintCopy(tllint *dest, tllint src) {
        tll_foreach(src, it) {
                tll_push_back(*dest, it->item);
        }
}

bool tllIntEqual(tllint a, tllint b) {
        int32 lenCmp = tll_length(a) - tll_length(b);
        // if length arent equal, return false
        if (lenCmp != 0) return false;
        
        // check if evert element of a is in b
        tll_foreach(a, ait) {
                bool present = false;
                tll_foreach(b, bit) {
                        if (ait->item == bit->item) {
                                present = true;
                                break;
                        }
                }
                if (!present)
                        return false;
        }
        return true;
}

int getIndex(int32 numV, struct valve valves[], int32 id) {
        for (int i = 0; i < numV; i++) {
                if (valves[i].id == id)
                        return i;
        }
        return -1;
}

void getDistances(int32 numV, struct valve valves[], int32 dists[numV][numV]) {
        // Initaloze array
        MAKE_LOOP(i, numV, j, numV)
                dists[i][j] = INT32_MAX;

        // Set neighbors to 1
        for (int i = 0; i < numV; i++) {
                struct valve valve = valves[i];
                int vIndex = valve.index;
                dists[vIndex][vIndex] = 0;
                tll_foreach(valve.neighbors, vit) {
                        int nIndex = getIndex(numV, valves, vit->item);
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

void unopenedCopy(tllint *dest, tllint src, int32 remove) {
        tll_foreach(src, it) {
                if (it->item!= remove)
                        tll_push_back(*dest, it->item);
        }
}

int32 waitTillEnd(struct state state) {
        int32 timeLeft = TIME_LIMIT - state.elapsed;
        return state.relieved + (state.flowRate * timeLeft);
}

int32 waitTillEnd2(struct state2 state) {
        int32 timeLeft = TIME_LIMIT - state.elapsed;
        return state.relieved + (state.flowRate * timeLeft);
}

int32 getMaxFlow(int32 numV, struct valve valves[],
                int32 distances[numV][numV], int numParties) {
        tllstate stateQueue = tll_init();

        // Add inital state to the queue
        struct state initState;
        initState.opened = (tllint)tll_init();
        initState.cur = getIndex(numV, valves, 0); // Valve "AA" has id of 0
        initState.cur2 = initState.cur;
        initState.elapsed = 0;
        initState.flowRate = 0;
        initState.relieved = 0;
        initState.unopened = (tllint)tll_init();
        for (int i = 0; i < numV; i++) {
                if (valves[i].flow != 0)
                        tll_push_back(initState.unopened, valves[i].index);
        }

        tll_push_back(stateQueue, initState);

        int32 max_relieved = 0;
        int64 count = 0;
        while (tll_length(stateQueue) > 0) {
                struct state curState = tll_pop_front(stateQueue);
                if (numParties == 2 && count % 10000 == 0)
                        printState(curState, true);
                count++;

                // if all flowing valves are open, wait until end
                if (tll_length(curState.unopened) == 0) {
                        int32 totalRelieved = waitTillEnd(curState);
                        if (totalRelieved > max_relieved)
                                max_relieved = totalRelieved;
                        continue;
                }

                tll_foreach(curState.unopened, it) {
                        int32 newCur = it->item;
                        struct valve valve = valves[newCur];
                        if (valve.flow <= 0)
                                continue;

                        int32 dist = distances[curState.cur][newCur] + 1;
                        int32 newElapsed = curState.elapsed + dist;

                        // if opening the valve takes too long, wait to end
                        if (newElapsed > TIME_LIMIT) {
                                int32 totalRelieved = waitTillEnd(curState);
                                if (totalRelieved > max_relieved)
                                        max_relieved = totalRelieved;
                                continue;
                        }

                        int32 newRelieved = curState.relieved +
                                curState.flowRate * dist;
                        int32 newFlowRate = curState.flowRate + valve.flow;

                        tllint newOpened = tll_init();
                        tllintCopy(&newOpened, curState.opened);
                        tll_push_back(newOpened, newCur);

                        tllint newUnopened = tll_init();
                        unopenedCopy(&newUnopened, curState.unopened, newCur);

                        struct state newState;
                        newState.opened = newOpened;
                        newState.unopened = newUnopened;
                        newState.cur = newCur;
                        newState.elapsed = newElapsed;
                        newState.flowRate = newFlowRate;
                        newState.relieved = newRelieved;
                        tll_push_back(stateQueue, newState);
                }
        }

        return max_relieved;
}

void part1(llist *ll) {
        int numV = ll->length;
        struct valve valveArr[numV];

        llNode *current = ll->head;
        int index = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                // get valve id
                strtok(str, " ");
                char *valve = strtok(NULL, " ");
                strncpy(valveArr[index].name, valve, 3);
                valveArr[index].id = valve2ID(valve);
                valveArr[index].index = index;

                // Get Valve flow rate
                strtok(NULL, "=");
                char *flowStr = strtok(NULL, ";");
                valveArr[index].flow = strtol(flowStr, (char**)NULL, 10);


                // Get valve neighbors
                valveArr[index].neighbors = (tllint)tll_init();
                for (int i = 0; i < 4; i++)
                        strtok(NULL, " ");
                char *next = strtok(NULL, ",");
                while (next != NULL) {
                        int32 nextId = valve2ID(next);
                        tll_push_back(valveArr[index].neighbors, nextId);
                        next = strtok(NULL, " ");
                }

                current = current->next;
                index++;
        }
        // printValves(numV, valveArr);

        int32 distances[numV][numV];
        getDistances(numV, valveArr, distances);
        // printDists(numV, distances);

        int32 maxFlow = getMaxFlow(numV, valveArr, distances, 1);

        printf("Part 1: Max Flow = %d\n\n", maxFlow);
}

void part2(llist *ll) {
        int numV = ll->length;
        struct valve valveArr[numV];

        llNode *current = ll->head;
        int index = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                // get valve id
                strtok(str, " ");
                char *valve = strtok(NULL, " ");
                strncpy(valveArr[index].name, valve, 3);
                valveArr[index].id = valve2ID(valve);
                valveArr[index].index = index;

                // Get Valve flow rate
                strtok(NULL, "=");
                char *flowStr = strtok(NULL, ";");
                valveArr[index].flow = strtol(flowStr, (char**)NULL, 10);


                // Get valve neighbors
                valveArr[index].neighbors = (tllint)tll_init();
                for (int i = 0; i < 4; i++)
                        strtok(NULL, " ");
                char *next = strtok(NULL, ",");
                while (next != NULL) {
                        int32 nextId = valve2ID(next);
                        tll_push_back(valveArr[index].neighbors, nextId);
                        next = strtok(NULL, " ");
                }

                current = current->next;
                index++;
        }
        // printValves(numV, valveArr);

        int32 distances[numV][numV];
        getDistances(numV, valveArr, distances);
        // printDists(numV, distances);

        int32 maxFlow = getMaxFlow(numV, valveArr, distances, 2);

        printf("Part 2: Max Flow = %d\n\n", maxFlow);
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

