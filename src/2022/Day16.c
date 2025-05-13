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
        for (int i = 0; i < num; i++) {
                for (int j = 0; j < num; j++) {
                        int n = dists[i][j];
                        if (n != 1000000)
                                printf("%d ", dists[i][j]);
                        else
                                printf("* ");
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

void part1(llist *ll) {
        initHashmap();
        int numValves = ll->length;
        int distances[numValves][numValves];
        MAKE_LOOP(i, numValves, j, numValves)
                distances[i][j] = 1000000;
        int flow[numValves];

        llNode *current = ll->head;
        int valvesSeen = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                // get valve id and index
                strtok(str, " ");
                int id = valve2ID(strtok(NULL, " "));
                int index = valvesSeen;
                if (hashMap[id] == -1) {
                        hashMap[id] = valvesSeen;
                        valvesSeen++;
                } else {
                        index = hashMap[id];
                }
                distances[index][index] = 0;

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

        floydWarshallAlg(numValves, distances);
        printDists(numValves, distances);

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

