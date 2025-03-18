/*************************************************
 *File----------Day2.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Mar 21, 2024 11:29:07 EDT
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
        int idSum = 0;
        int id = 1;
        char *colors[3] = {"red", "green", "blue"};
        int colorCount[3] = {12, 13, 14};
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (str[0] == '\0') break;

                bool invalid = false;
                char *token = strtok(str, ":");   // Remove First Part of string
                token = strtok(NULL, ",;");

                while (token != NULL) {
                        char *color;
                        int num = strtol(token, &color, 10);    // Split into number and color
                        memmove(color, color + 1, strlen(color));   // Remove first space from color;
                        for (int i = 0; i < 3; i++) {
                                if (strcmp(color, colors[i]) == 0 && num > colorCount[i]) {
                                        invalid = true; 
                                        break;
                                }
                        }
                        if (invalid) {
                                break;
                        }
                        token = strtok(NULL, ",;");     // split by , or ;
                }

                if (!invalid) {
                        idSum += id;
                }

                current = current->next;
                id++;
        }
        printf("Part 1: Sum = %d\n", idSum);
}

void part2(llist *ll) {
        int powersum = 0;
        char *colors[3] = {"red", "green", "blue"};
        llNode *current = ll->head;
        int i = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (str[0] == '\0') break;

                char *token = strtok(str, ":");   // Remove First Part of string
                token = strtok(NULL, ",;");
                int colorCount[3] = {0, 0, 0};

                while (token != NULL) {
                        char *color;
                        int num = strtol(token, &color, 10);    // Split into number and color
                        memmove(color, color + 1, strlen(color));   // Remove first space from color;
                        for (int i = 0; i < 3; i++) {
                                if (strcmp(color, colors[i]) == 0 && num > colorCount[i]) {
                                        colorCount[i] = num;
                                        break;
                                }
                        }
                        token = strtok(NULL, ",;");     // split by , or ;
                }

                int power = colorCount[0] * colorCount[1] * colorCount[2];
                powersum += power;

                // printf("%d; Red: %d, Green: %d, Blue: %d; Product: %d; Sum: %d\n",
                // i, colorCount[0], colorCount[1], colorCount[2], power, powersum);

                current = current->next;
                i++;
        }
        printf("Part 2: Power Sum = %d\n", powersum);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2023/Day2.txt");
        } else {
                ll = getInputFile("assets/inputs/2023/Day2.txt");
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

