/*************************************************
 *File----------Day2.c
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
        printf("\nPart 1: \n");

        int numSafe = 0;
        for (int i = 0; i < ll->length; i++) {
                int cur = reports[i][0];
                bool isSafe = 1;
                bool isIncreasing = reports[i][0] < reports[i][1];
                printf("%d ", reports[i][0]);
                for (int j = 1; j < MAX_REPORT_LEN; j++) {
                        if(reports[i][j] == -1)
                                break;

                        // Check if next value is different and within 3 
                        // of previous value
                        int diff = abs(cur - reports[i][j]);
                        if (diff == 0 || diff > 3) {
                                isSafe = false;
                                break;
                        }
                        // Check if the report remains increasing or decreasing
                        if ((cur < reports[i][j]) != isIncreasing) {
                                isSafe = false;
                                break;
                        }

                        cur = reports[i][j];
                        printf("%d ", reports[i][j]);
                }
                if (isSafe == 1) {
                        printf("Safe\n");
                        numSafe++;
                } else {
                        printf("unsafe\n");
                }
        }

        printf("Part1: Safe Reports: %d\n", numSafe);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                current = current->next;
        }
        printf("\nPart 2: \n");
}

int main(int argc, char *argv[]) {
        llist *ll = getInputFile("assets/2024/Day2.txt");
        // llist *ll = getInputFile("assets/test.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}


