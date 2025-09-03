/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Sep 03, 2025 17:59:17 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096
#define SEED 0x12345678

typedef enum moduleType {
        NONE,
        BROAD,
        FLIP,
        CONJ
} moduleType;

typedef enum signalType {
        LOW,
        HIGH
} signalType;

typedef tll(uint16) tlluint16;

union moduleState {
        bool on;
        tlluint16 inOn;
};

typedef struct module {
        moduleType type;
        tlluint16 outputs;
        union moduleState state;
} module;

typedef struct state {
        signalType signal;
        uint16 src;
        uint16 dest;
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

// FNV Hashing Algorithm - H is seed
uint16 FNV16(const char *key, uint32 h) {
        h ^= 2166136261UL;
        const uint8 *data = (const uint8*)key;
        for (int i=0; data[i]!='\0'; i++) {
                h ^= data[i];
                h *= 16777619;
        }
        return (uint16)h;
}

void evalBroadcaster(module *m, signalType sig, uint16 src, tllstate *que) {
        tll_foreach(m->outputs, it) {
                state newS = {sig, src, it->item};
                tll_push_back(*que, newS);
        }
}

void evalFlipFlop(module *m, signalType sig, uint16 src, tllstate *que) {
        if (sig == HIGH)
                return;
        m->state.on = !m->state.on;
        tll_foreach(m->outputs, it) {
                state newS = {m->state.on, src, it->item};
                tll_push_back(*que, newS);
        }
}

void updateSignal(tlluint16 *signals, uint16 dest, signalType signal) {
        bool first = true;
        bool present = false;
        tll_foreach(*signals, it) {
                if (first) {
                        first = false;
                        continue;
                }
                if (it->item == dest && signal == LOW)
                        tll_remove(*signals, it);
                else if (it->item == dest && signal == HIGH)
                        present = true;
        }
        if (signal == HIGH && !present)
                tll_push_back(*signals, dest);
}

void evalConjunction(module *m, signalType sig, uint16 src,
                uint16 cur, tllstate *que) {
        updateSignal(&m->state.inOn, src, sig);
        signalType newSig = HIGH;
        if ((int)tll_length(m->state.inOn) == tll_front(m->state.inOn)+1)
                newSig = LOW;
        tll_foreach(m->outputs, it) {
                state newS = {newSig, cur, it->item};
                tll_push_back(*que, newS);
        }
}

ivec2 sendPulse(module modules[]) {
        const uint16 StartModule = FNV16("broadcaster", SEED);

        tllstate queue = tll_init();
        state initState = {LOW, 0, StartModule};
        tll_push_back(queue, initState);

        ivec2 sigCount = {0, 0};        // HIGH, LOW
        while (tll_length(queue) > 0) {
                state curS = tll_pop_front(queue);
                module *curM = &modules[curS.dest];

                if (curS.signal == HIGH) sigCount.x++;
                if (curS.signal == LOW) sigCount.y++;

                switch (curM->type) {
                case NONE:
                        break;
                case BROAD:
                        evalBroadcaster(curM, curS.signal, curS.dest, &queue);
                        break;
                case FLIP:
                        evalFlipFlop(curM, curS.signal, curS.dest, &queue);
                        break;
                case CONJ:
                        evalConjunction(curM, curS.signal, curS.src,
                                        curS.dest, &queue);
                        break;
                }
        }

        return sigCount;
}

void initConj(module modules[], tlluint16 moduleIDs) {
        tll_foreach(moduleIDs, it) {
                module *m = &modules[it->item];
                tll_foreach(m->outputs, it2) {
                        module *mc = &modules[it2->item];
                        if (mc->type != CONJ)
                                continue;
                        if (tll_length(mc->state.inOn) == 0)
                                tll_push_back(mc->state.inOn, 1);
                        else
                                mc->state.inOn.head->item++;
                }
        }
}

void part1(llist *ll) {
        module modules[(uint32)UINT16_MAX+1] = {0};
        tlluint16 moduleIDs = tll_init();

        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                moduleType mt = NONE;
                uint16 moduleID;
                if (str[0] == '%' || str[0] == '&') {
                        mt = str[0]=='%' ? FLIP : CONJ;
                        char *name = strtok(str + 1, " ");
                        moduleID = FNV16(name, SEED);
                        debugP("%s: %u\n", name, moduleID);
                } else {
                        char *name = strtok(str, " ");
                        if (strcmp(name, "broadcaster") == 0)
                                mt = BROAD;
                        moduleID = FNV16(name, SEED);
                        debugP("%s: %u\n", name, moduleID);
                }
                if (modules[moduleID].type != NONE)
                        printf("COLLISION: %u\n", moduleID);
                modules[moduleID].type = mt;
                tll_push_back(moduleIDs, moduleID);

                strtok(NULL, " ");
                char *out = strtok(NULL, ", ");
                while (out != NULL) {
                        uint16 outID = FNV16(out, SEED);
                        tll_push_back(modules[moduleID].outputs, outID);
                        out = strtok(NULL, ", ");
                }

                current = current->next;
        }
        debugP("\n");

        initConj(modules, moduleIDs);

        ivec2 sigTotal = {0, 0};
        for (int i=0; i<1000; i++) {
                ivec2 sigCount = sendPulse(modules);
                // printf("%d, %d\n", sigCount.x, sigCount.y);
                sigTotal = addIVec2(sigTotal, sigCount);
        }
        debugP("HIGH: %d, LOW: %d\n", sigTotal.x, sigTotal.y);

        int64 signals = (int64)sigTotal.x * sigTotal.y;

        printf("Part 1: %ld\n\n", signals);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                // char *file = "assets/tests/2023/Day20.txt";
                char *file = "assets/tests/2023/Day20b.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day20.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
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

