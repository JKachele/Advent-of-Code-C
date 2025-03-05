/*************************************************
 *File----------Day6.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Apr 22, 2024 07:44:02 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

void removeSpaces(char *str) {
        int count = 0;
        for (int i = 0; str[i]; i++) {
                if (str[i] != ' ') {
                        str[count++] = str[i];
                }
        }
        str[count] = '\0';
}

void part1(llist *ll) {
        char timeStr[BUFFER_SIZE], distStr[BUFFER_SIZE];
        strncpy(timeStr, (char*)ll->head->data, BUFFER_SIZE);
        strncpy(distStr, (char*)ll->tail->data, BUFFER_SIZE);

        bool colonFound = false;
        bool numFound = false;
        int numraces = 0;
        for (int i = 0; i < strlen(timeStr); i++) {
                char c = timeStr[i];
                if (!colonFound && c == ':') {
                        colonFound = true;
                } else if (colonFound) {
                        if (!numFound && c != ' ') {
                                numFound = true;
                                numraces++;
                        } else if (numFound && c == ' ') {
                                numFound = false;
                        }
                }
        }

        int raceTimes[numraces];
        strtok(timeStr, " ");
        for (int i = 0; i < numraces; i++) {
                raceTimes[i] = strtol(strtok(NULL, " "), NULL, 10);
        }

        int distances[numraces];
        strtok(distStr, " ");
        for (int i = 0; i < numraces; i++) {
                distances[i] = strtol(strtok(NULL, " "), NULL, 10);
        }

        int recordProduct = 1;
        for (int i = 0; i < numraces; i++) {
                int time = raceTimes[i];
                int record = distances[i];
                int recordNumber = 0;
                for (int t = 0; t < time; t++) {
                        int distance = t * (time - t);
                        if (distance > record) {
                                recordNumber++;
                        }
                }
                recordProduct = recordProduct * recordNumber;
        }
        printf("Part 1: Record product = %d\n", recordProduct);
}

void part2(llist *ll) {
        char timeInput[BUFFER_SIZE], distInput[BUFFER_SIZE];
        strncpy(timeInput, (char*)ll->head->data, BUFFER_SIZE);
        strncpy(distInput, (char*)ll->tail->data, BUFFER_SIZE);

        strtok(timeInput, " ");
        char *timeStr = strtok(NULL, "");
        removeSpaces(timeStr);
        long time = strtol(timeStr, NULL, 10);

        strtok(distInput, " ");
        char *distStr = strtok(NULL, "");
        removeSpaces(distStr);
        long record = strtol(distStr, NULL, 10);
        // printf("Time: %ld\tDistance: %ld\n", time, record);

        int recordCount = 0;
        for (long i = 0; i < time; i++) {
                long distance = i * (time - i);
                if (distance > record) {
                        recordCount++;
                }
        }

        printf("Part 2: Record Count = %d\n", recordCount);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2023/Day6.txt");
        } else {
                ll = getInputFile("assets/2023/Day6.txt");
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

