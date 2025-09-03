/*************************************************
 *File----------Day19.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Sep 02, 2025 12:38:44 UTC
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

typedef enum category {
        COOL,
        MUSICAL,
        AERO,
        SHINY,
} category;

typedef struct rule {
        bool condition;
        category cat;
        bool gt;
        int32 num;
        int32 dest;     // Hash of destination, 0 is accept, 1 is reject
} rule;

typedef tll(rule) tllrule;

typedef struct part {
        int32 values[4];
} part;

typedef tll(part) tllpart;

typedef tll(ivec2) tllivec2;

typedef struct state {
        ivec2 ranges[4];
        int32 rule;
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

void printRules(tllrule rules) {
        printf("[ ");
        tll_foreach(rules, it) {
                rule r = it->item;
                if (!r.condition) {
                        printf("%u ", r.dest);
                        continue;
                }
                switch (r.cat) {
                case COOL:
                        printf("x");
                        break;
                case MUSICAL:
                        printf("m");
                        break;
                case AERO:
                        printf("a");
                        break;
                case SHINY:
                        printf("s");
                        break;
                }
                printf("%c", r.gt? '>' : '<');
                printf("%d:%u ", r.num, r.dest);
        }
        printf("]\n");
}

void printPart(part p) {
        printf("x=%d  ", p.values[0]);
        printf("m=%d  ", p.values[1]);
        printf("a=%d  ", p.values[2]);
        printf("s=%d\n", p.values[3]);
}

void printParts(tllpart parts) {
        tll_foreach(parts, it) {
                printPart(it->item);
        }
}

// FNV Hashing Algorithm - H is seed
uint32 FNV(const char *key, uint32 h) {
        h ^= 2166136261UL;
        const uint8 *data = (const uint8*)key;
        for (int i=0; data[i]!='\0'; i++) {
                h ^= data[i];
                h *= 16777619;
        }
        return h;
}

rule parseRule(char *str) {
        char ruleStr[10];
        strncpy(ruleStr, str, 10);
        rule r = {0};

        if (strchr(ruleStr, ':') == NULL) {
                r.condition = false;
                uint16 dest = (uint16)FNV(ruleStr, SEED);
                r.dest = dest;
                return r;
        }

        // printf("%s\n", ruleStr);
        r.condition = true;
        char *greater = strchr(ruleStr, '>');
        char *less = strchr(ruleStr, '<');
        char *opp = greater != NULL ? greater : less;
        // printf("%s - %s\n", ruleStr, opp);
        r.gt = opp[0] == '>';
        opp[0] = '\0';

        char *cat = ruleStr;
        switch (cat[0]) {
        case 'x':
                r.cat = COOL;
                break;
        case 'm':
                r.cat = MUSICAL;
                break;
        case 'a':
                r.cat = AERO;
                break;
        case 's':
                r.cat = SHINY;
                break;
        }

        char *num = opp + 1;
        char *colon = strchr(num, ':');
        colon[0] = '\0';
        r.num = strtol(num, (char**)NULL, 10);

        char *dest = colon + 1;
        r.dest = (uint16)FNV(dest, SEED);

        return r;
}

void parseRules(char *workflow, tllrule rules[]) {
                char *token = strtok(workflow, "{");
                uint16 wfid = (uint16)FNV(token, SEED); // Workflow ID
                if(rules[wfid].head != NULL) {
                        printf("COLLISION on %s!\n", token);
                        exit(1);
                }
                rules[wfid] = (tllrule)tll_init();
                debugP("%s: %u\n", token, wfid);

                // printf("%s\n", token);
                token = strtok(NULL, ",}");
                while (token != NULL) {
                        rule r = parseRule(token);

                        tll_push_back(rules[wfid], r);
                        token = strtok(NULL, ",}");
                }
                // printRules(rules[wfid]);
}

part parsePart(char *str) {
        part p = {0};

        char *token = strtok(str, "=");
        token = strtok(NULL, ",");
        p.values[0] = strtol(token, (char**)NULL, 10);

        for (int i=1; i<4; i++) {
                token = strtok(NULL, "=");
                token = strtok(NULL, ",}");
                p.values[i] = strtol(token, (char**)NULL, 10);
        }

        return p;
}

uint16 nextWF(part p, tllrule wf) {
        tll_foreach(wf, it) {
                rule r = it->item;
                if (!r.condition)
                        return r.dest;

                int32 partVal = p.values[r.cat];
                bool greater = partVal > r.num;
                bool less = partVal < r.num;

                if (greater == r.gt && less != r.gt)
                        return r.dest;
        }
        return 0;
}

bool partAccepted(part p, tllrule rules[]) {
        const uint16 StartWF = (uint16)FNV("in", SEED);
        const uint16 Accept = (uint16)FNV("A", SEED);
        const uint16 Reject = (uint16)FNV("R", SEED);

        uint16 currWF = StartWF;
        while (currWF != Accept && currWF != Reject) {
                tllrule ruleList = rules[currWF];
                uint16 nextWFID = nextWF(p, ruleList);
                // printf("NextWF: %u\n", nextWFID);
                currWF = nextWFID;
        }

        return currWF == Accept;
}

void copyRanges(ivec2 ranges[], ivec2 newRanges[]) {
        for (int i=0; i<4; i++) {
                newRanges[i] = ranges[i];
        }
}

uint64 addAccept(ivec2 ranges[]) {
        uint64 accepted = 1;
        for (int i=0; i<4; i++) {
                accepted *= (ranges[i].y - ranges[i].x) + 1;
        }
        return accepted;
}

ivec2 adjustRange(ivec2 range, int32 limit, bool greater, bool leftover) {
        if (range.x > limit || range.y < limit)
                return (ivec2){-1, -1};

        ivec2 newRange = range;
        if (greater) {
                if (leftover)
                        newRange.y = limit;
                else
                        newRange.x = limit + 1;
        } else {
                if (leftover)
                        newRange.x = limit;
                else
                        newRange.y = limit - 1;
        }
        return newRange;
}

uint64 maxPartsAccepted(tllrule rules[]) {
        const uint16 StartWF = (uint16)FNV("in", SEED);
        const uint16 Accept = (uint16)FNV("A", SEED);
        const uint16 Reject = (uint16)FNV("R", SEED);

        uint64 numAccepted = 0;

        tllstate queue = tll_init();
        state initState;
        initState.rule = StartWF;
        for (int i=0; i<4; i++) initState.ranges[i] = (ivec2){1, 4000};
        tll_push_back(queue, initState);

        while (tll_length(queue) > 0) {
                state curS = tll_pop_front(queue);

                if (curS.rule == Accept) {
                        numAccepted += addAccept(curS.ranges);
                        continue;
                } else if (curS.rule == Reject) {
                        continue;
                }

                tllrule curWF = rules[curS.rule];
                ivec2 leftover[4] = {0};
                copyRanges(curS.ranges, leftover);
                tll_foreach(curWF, it) {
                        rule r = it->item;
                        state nextS = curS;
                        copyRanges(leftover, nextS.ranges);

                        if (r.condition == false) {
                                nextS.rule = r.dest;
                                tll_push_back(queue, nextS);
                                continue;
                        }

                        ivec2 curR = nextS.ranges[r.cat];
                        ivec2 nextR = adjustRange(curR, r.num, r.gt, false);
                        if (nextR.x == -1 || nextR.y == -1)
                                continue;
                        nextS.ranges[r.cat] = nextR;
                        nextS.rule = r.dest;
                        tll_push_back(queue, nextS);

                        // Set leftover ranges
                        leftover[r.cat] = adjustRange(curR, r.num, r.gt, true);
                }
        }

        return numAccepted;
}

void part1(llist *ll) {
        tllrule rules[(uint32)UINT16_MAX + 1] = {0};
        tllpart parts = tll_init();

        // Parse Workflows
        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                if (strlen(str) == 0)
                        break;
                parseRules(str, rules);
                current = current->next;
        }
        debugP("A: %u\n", (uint16)FNV("A", SEED));
        debugP("R: %u\n\n", (uint16)FNV("R", SEED));

        // parse parts
        current = current->next;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                part p = parsePart(str);
                tll_push_back(parts, p);
                current = current->next;
        }
        // printParts(parts);

        int64 ratingSum = 0;
        int32 i = 0;
        tll_foreach(parts, it) {
                bool accepted = partAccepted(it->item, rules);
                // printf("%d %s\n", i, accepted? "Accept" : "Reject");
                // printPart(it->item);
                if (!accepted) {i++; continue;}
                for (int i=0; i<4; i++) {
                        ratingSum += it->item.values[i];
                }
                i++;
        }

        printf("Part 1: %ld\n\n", ratingSum);
}

void part2(llist *ll) {
        tllrule rules[(uint32)UINT16_MAX + 1] = {0};

        // Parse Workflows
        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                if (strlen(str) == 0)
                        break;
                parseRules(str, rules);
                current = current->next;
        }
        debugP("A: %u\n", (uint16)FNV("A", SEED));
        debugP("R: %u\n\n", (uint16)FNV("R", SEED));

        uint64 numAccepted = maxPartsAccepted(rules);

        printf("Part 2: %lu\n", numAccepted);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day19.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day19.txt";
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

