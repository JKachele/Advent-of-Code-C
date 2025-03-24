/*************************************************
 *File----------Day4.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 19, 2025 16:49:04 EDT
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
        int total = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                ivec2 area1;
                ivec2 area2;

                area1.x = strtol(strtok(str, "-"), (char**)NULL, 10);
                area1.y = strtol(strtok(NULL, ","), (char**)NULL, 10);
                area2.x = strtol(strtok(NULL, "-"), (char**)NULL, 10);
                area2.y = strtol(strtok(NULL, ""), (char**)NULL, 10);

                if (area1.x <= area2.x && area1.y >= area2.y)
                        total++;
                else if (area2.x <= area1.x && area2.y >= area1.y)
                        total++;

                current = current->next;
        }
        printf("Part 1: Num pairs with full overlap: %d\n\n", total);
}

void part2(llist *ll) {
        int total = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                ivec2 area1;
                ivec2 area2;

                area1.x = strtol(strtok(str, "-"), (char**)NULL, 10);
                area1.y = strtol(strtok(NULL, ","), (char**)NULL, 10);
                area2.x = strtol(strtok(NULL, "-"), (char**)NULL, 10);
                area2.y = strtol(strtok(NULL, ""), (char**)NULL, 10);

                if (area1.x <= area2.x && area1.y >= area2.x)
                        total++;
                else if (area2.x <= area1.x && area2.y >= area1.x)
                        total++;

                current = current->next;
        }
        printf("Part 2: Num pairs with any overlap: %d\n\n", total);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day4.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day4.txt");
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

