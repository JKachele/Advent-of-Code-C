/*************************************************
 *File----------Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Dec 04, 2024 11:39:49 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include<stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

bool reportSafe(int* report, int reportLen) {
        int cur = report[0];
        bool isSafe = 1;
        bool isIncreasing = report[0] < report[1];
        // printf("%d ", reports[i][0]);
        for (int i = 1; i < reportLen; i++) {
                if(report[i] == -1)
                        break;

                // Check if next value is different and within 3 
                // of previous value
                int diff = abs(cur - report[i]);
                if (diff == 0 || diff > 3) {
                        isSafe = false;
                        break;
                }
                // Check if the report remains increasing or decreasing
                if ((cur < report[i]) != isIncreasing) {
                        isSafe = false;
                        break;
                }

                cur = report[i];
                // printf("%d ", reports[i][j]);
        }
        return isSafe;
}

// Part2: Brute Force - Iterating through each removal
bool reportSafe2(int* report, int reportLen) {
        // First try unaltered report
        if(reportSafe(report, reportLen))
                return true;

        // If unaltered report is unsafe, remove an element and try again
        for (int i = 0; i < reportLen; i++) {
                int newReport[reportLen];
                int oldIndex = 0;
                int newIndex = 0;
                do {
                        if (oldIndex == i) {
                                oldIndex++;
                                continue;
                        }
                        newReport[newIndex] = report[oldIndex];
                        oldIndex++;
                        newIndex++;
                } while (report[oldIndex - 1] != -1);

                if(reportSafe(newReport, reportLen))
                        return true;
        }
        return false;
}

void part1(llist *ll) {
        const int MAX_REPORT_LEN = 10;

        int reports[ll->length][MAX_REPORT_LEN];

        int reportNum = 0;
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                char* entry = strtok(str, " ");
                int entryNum = 0;
                while (entry != NULL) {
                        reports[reportNum][entryNum] = 
                                strtol(entry, (char**)NULL, 10);
                        entry = strtok(NULL, " ");
                        entryNum++;
                }
                reports[reportNum][entryNum] = -1; // Set -1 as terminator

                current = current->next;
                reportNum++;
        }

        int numSafe = 0;
        for (int i = 0; i < ll->length; i++) {
                if(reportSafe(reports[i], MAX_REPORT_LEN))
                        numSafe++;
        }

        printf("Part1: Safe Reports: %d\n", numSafe);
}

void part2(llist *ll) {
        const int MAX_REPORT_LEN = 10;

        int reports[ll->length][MAX_REPORT_LEN];

        int reportNum = 0;
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                char* entry = strtok(str, " ");
                int entryNum = 0;
                while (entry != NULL) {
                        reports[reportNum][entryNum] = 
                                strtol(entry, (char**)NULL, 10);
                        entry = strtok(NULL, " ");
                        entryNum++;
                }
                reports[reportNum][entryNum] = -1; // Set -1 as terminator

                current = current->next;
                reportNum++;
        }

        int numSafe = 0;
        for (int i = 0; i < ll->length; i++) {
                if(reportSafe2(reports[i], MAX_REPORT_LEN))
                        numSafe++;
        }

        printf("Part2: Safe Reports: %d\n", numSafe);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day02.txt");
        else
                ll = getInputFile("assets/inputs/2024/Day02.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


