/*************************************************
 *File----------Day2.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Mar 21, 2024 11:29:07 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

void part1(linkedList_s ll) {
    int idSum = 0;
    int id = 1;
    char *colors[3] = {"red", "green", "blue"};
    int colorCount[3] = {12, 13, 14};
    node_s *current = ll.head;
    while(current != NULL) {
        char *str;
        strncpy(str, current->str, BUFFER_SIZE);
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

void part2(linkedList_s ll) {
    int powersum = 0;
    char *colors[3] = {"red", "green", "blue"};
    node_s *current = ll.head;
    while(current != NULL) {
        char *str;
        strncpy(str, current->str, BUFFER_SIZE);
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

        powersum += colorCount[0] * colorCount[1] * colorCount[2];

        current = current->next;
    }
    printf("Part 2: Power Sum = %d\n", powersum);
}

int main(int argc, char *argv[]) {
    linkedList_s ll = getInputFile("assets/2023/Day2.txt");
    // linkedList_s ll = getInputFile("assets/test.txt");
    // printSNodeList(ll.head);

    part1(ll);
    part2(ll);

    return 0;
}


