/*************************************************
 *File----------Day01.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 18, 2025 18:23:55 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

void part1(llist *ll) {
        int32 mostCals = 0;

        llNode *current = ll->head;
        int32 curTotal = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (strlen(str) == 0) {
                        if (curTotal > mostCals)
                                mostCals = curTotal;
                        curTotal = 0;
                } else {
                        curTotal += strtol(str, (char**)NULL, 10);

                }

                current = current->next;
        }
        if (curTotal > mostCals)
                mostCals = curTotal;

        printf("Part 1: Most Calories: %d\n\n", mostCals);
}

void part2(llist *ll) {
        int32 mostCals1 = 0;
        int32 mostCals2 = 0;
        int32 mostCals3 = 0;

        llNode *current = ll->head;
        int32 curTotal = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (strlen(str) == 0) {
                        if (curTotal > mostCals1) {
                                mostCals3 = mostCals2;
                                mostCals2 = mostCals1;
                                mostCals1 = curTotal;
                        } else if (curTotal > mostCals2) {
                                mostCals3 = mostCals2;
                                mostCals2 = curTotal;
                        } else if (curTotal > mostCals3) {
                                mostCals3 = curTotal;
                        }
                        curTotal = 0;
                } else {
                        curTotal += strtol(str, (char**)NULL, 10);
                }

                current = current->next;
        }
        if (curTotal > mostCals1) {
                mostCals3 = mostCals2;
                mostCals2 = mostCals1;
                mostCals1 = curTotal;
        } else if (curTotal > mostCals2) {
                mostCals3 = mostCals2;
                mostCals2 = curTotal;
        } else if (curTotal > mostCals3) {
                mostCals3 = curTotal;
        }

        int32 total = mostCals1 + mostCals2 + mostCals3;

        printf("Part 2: Calories from top 3: %d\n\n", total);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day01.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day01.txt");
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

