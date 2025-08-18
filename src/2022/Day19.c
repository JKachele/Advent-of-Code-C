/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Aug 18, 2025 19:08:22 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

// [Ore, Clay, Obsidion, Geode]
typedef struct {
        int16 ore[4];
        int16 clay[4];
        int16 obs[4];
        int16 geode[4];
        int16 max[4];
} blueprint;

typedef struct {
        int32 res[4];
        int32 bots[4];
        int32 timeElapsed;
} state;

typedef tll(state) tllstate;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printLen(tllstate states) {
        printf("%lu\n", tll_length(states));
}

bool enoughRes(int16 bp[4], int32 res[4]) {
        for (int i=0; i<4; i++) {
                if (res[i] < bp[i])
                        return false;
        }
        return true;
}

int32 timeToBuild(int16 cost[4], state state) {
        int32 timeTillBuild = 0;
        for (int i=0; i<4; i++) {
                if (cost[i] <= state.res[i]) continue;
                int buildTime = ceil((cost[i] - state.res[i]) /
                                (float)state.bots[i]);
                if (buildTime > timeTillBuild)
                        timeTillBuild = buildTime;
        }
        return timeTillBuild + 1;
}

state scheduleOre(blueprint bp, state inState, int32 timeLimit) {
        state newState = inState;
        int32 timeRequired = timeToBuild(bp.ore, inState);
        newState.timeElapsed += timeRequired;
        for (int i=0; i<4; i++)
                newState.res[i] = (inState.res[i] - bp.ore[i]) +
                        (inState.bots[i] * timeRequired);
        newState.bots[0]++;
        return newState;
}

state scheduleClay(blueprint bp, state inState, int32 timeLimit) {
        state newState = inState;
        int32 timeRequired = timeToBuild(bp.clay, inState);
        newState.timeElapsed += timeRequired;
        for (int i=0; i<4; i++)
                newState.res[i] = (inState.res[i] - bp.clay[i]) +
                        (inState.bots[i] * timeRequired);
        newState.bots[1]++;
        return newState;
}

state scheduleObs(blueprint bp, state inState, int32 timeLimit) {
        state newState = inState;
        int32 timeRequired = timeToBuild(bp.obs, inState);
        newState.timeElapsed += timeRequired;
        for (int i=0; i<4; i++)
                newState.res[i] = (inState.res[i] - bp.obs[i]) +
                        (inState.bots[i] * timeRequired);
        newState.bots[2]++;
        return newState;
}

state scheduleGeo(blueprint bp, state inState, int32 timeLimit) {
        state newState = inState;
        int32 timeRequired = timeToBuild(bp.geode, inState);
        newState.timeElapsed += timeRequired;
        for (int i=0; i<4; i++)
                newState.res[i] = (inState.res[i] - bp.geode[i]) +
                        (inState.bots[i] * timeRequired);
        newState.bots[3]++;
        return newState;
}

state timeLeft(state inState, int32 timeLimit) {
        state newState = inState;
        int32 timeRequired = timeLimit - inState.timeElapsed;
        newState.timeElapsed += timeRequired;
        for (int i=0; i<4; i++)
                newState.res[i] = inState.res[i] +
                        (inState.bots[i] * timeRequired);
        return newState;
}

int32 maxGeodes(blueprint bp, int32 timeLimit) {
        tllstate stateQueue = tll_init();

        state initState = {0};
        initState.bots[0] = 1;
        tll_push_back(stateQueue, initState);

        int32 maxGeodes = 0;
        while (tll_length(stateQueue) > 0) {
                state curState = tll_pop_front(stateQueue);

                if (curState.timeElapsed >= timeLimit) {
                        if (curState.res[3] > maxGeodes)
                                maxGeodes = curState.res[3];
                        continue;
                }

                // Ore bot: Requires ore
                if (bp.max[0] > curState.bots[0] && curState.bots[0] > 0) {
                        state newState =  scheduleOre(bp, curState, timeLimit);
                        if (newState.timeElapsed >= timeLimit) {
                                newState = timeLeft(curState, timeLimit);
                                if (newState.res[3] > maxGeodes)
                                        maxGeodes = newState.res[3];
                        } else {
                                tll_push_back(stateQueue, newState);
                        }
                }

                // Clay bot: Requires ore
                if (bp.max[1] > curState.bots[1] && curState.bots[0] > 0) {
                        state newState =  scheduleClay(bp, curState, timeLimit);
                        if (newState.timeElapsed >= timeLimit) {
                                newState = timeLeft(curState, timeLimit);
                                if (newState.res[3] > maxGeodes)
                                        maxGeodes = newState.res[3];
                        } else {
                                tll_push_back(stateQueue, newState);
                        }
                }

                // Obsidion bot: Requires ore and clay
                if (bp.max[1] > curState.bots[1] &&
                                curState.bots[0] > 0 && curState.bots[1] > 0) {
                        state newState =  scheduleObs(bp, curState, timeLimit);
                        if (newState.timeElapsed >= timeLimit) {
                                newState = timeLeft(curState, timeLimit);
                                if (newState.res[3] > maxGeodes)
                                        maxGeodes = newState.res[3];
                        } else {
                                tll_push_back(stateQueue, newState);
                        }
                }

                // Geode bot: Requires ore and obsidion
                if (bp.max[1] > curState.bots[1] &&
                                curState.bots[0] > 0 && curState.bots[2] > 0) {
                        state newState =  scheduleGeo(bp, curState, timeLimit);
                        if (newState.timeElapsed >= timeLimit) {
                                newState = timeLeft(curState, timeLimit);
                                if (newState.res[3] > maxGeodes)
                                        maxGeodes = newState.res[3];
                        } else {
                                tll_push_back(stateQueue, newState);
                        }
                }
        }

        return maxGeodes;
}

void getMaxes(blueprint *bp) {
        for (int i=0; i<4; i++) {
                int16 max = 0;
                if (bp->ore[i] > max) max = bp->ore[i];
                if (bp->clay[i] > max) max = bp->clay[i];
                if (bp->obs[i] > max) max = bp->obs[i];
                if (bp->geode[i] > max) max = bp->geode[i];
                bp->max[i] = max;
        }
}

void part1(llist *ll) {
        const int32 TimeLimit = 24;
        int16 numBp = ll->length;
        blueprint blueprints[numBp];
        memset(blueprints, 0, sizeof(blueprint) * numBp);

        llNode *current = ll->head;
        int16 index = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                char *token = strtok(str, " ");
                for (int i=0; i<6; i++) token = strtok(NULL, " ");
                blueprints[index].ore[0] = strtol(token, (char**)NULL, 10);

                for (int i=0; i<6; i++) token = strtok(NULL, " ");
                blueprints[index].clay[0] = strtol(token, (char**)NULL, 10);

                for (int i=0; i<6; i++) token = strtok(NULL, " ");
                blueprints[index].obs[0] = strtol(token, (char**)NULL, 10);

                for (int i=0; i<3; i++) token = strtok(NULL, " ");
                blueprints[index].obs[1] = strtol(token, (char**)NULL, 10);

                for (int i=0; i<6; i++) token = strtok(NULL, " ");
                blueprints[index].geode[0] = strtol(token, (char**)NULL, 10);

                for (int i=0; i<3; i++) token = strtok(NULL, " ");
                blueprints[index].geode[2] = strtol(token, (char**)NULL, 10);

                current = current->next;
                index++;
        }
        for (int i=0; i<numBp; i++) {
                getMaxes(&blueprints[i]);
        }

        int32 qualitySum = 0;
        for (int i=0; i<numBp; i++) {
                int32 geodes = maxGeodes(blueprints[i], TimeLimit);
                int32 id = i + 1;
                debugP("Blueprint %d: %d Geodes\n", id, geodes);
                int32 quality = geodes * id;
                qualitySum += quality;
        }

        printf("Part 1: Blueprint Quality Sum: %d\n\n", qualitySum);
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
                ll = getInputFile("assets/tests/2022/Day19.txt");
                Debug = true;
        } else {
                ll = getInputFile("assets/inputs/2022/Day19.txt");
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

