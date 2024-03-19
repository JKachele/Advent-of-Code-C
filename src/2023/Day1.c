/*************************************************
 *File----------Day1a
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 28, 2024 13:20:23 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

void replaceSubString(char *str, char *find, char *replace, char *output) {
    bool substr = false;

    str[strlen(str) - 1] = '\0';
}

void part1(linkedList_s ll) {
    int calibrationSum = 0;
    node_s *current = ll.head;
    while(current != NULL) {
        char *str = current->str;
        int digit1 = 0;
        int digit2 = 0;
        for (int i = 0; i < strlen(str); i++) {
            int charNum = str[i] - '0';
            if (charNum >= 0 && charNum <= 9) {
                digit1 = charNum;
                break;
            }
        }
        for (int i = strlen(str) - 1; i >= 0; i--) {
            int charNum = str[i] - '0';
            if (charNum >= 0 && charNum <= 9) {
                digit2 = charNum;
                break;
            }
        }
        calibrationSum += (digit1 * 10) + digit2;

        current = current->next;
    }

    printf("Part 1 Calibration Sum = %d\n", calibrationSum);
}

void part2(linkedList_s ll) {
    int calibrationSum = 0;
    node_s *current = ll.head;
    while(current != NULL) {
        char *str = current->str;

        current = current->next;
    }
    while(current != NULL) {
        char *str = current->str;
        int digit1 = 0;
        int digit2 = 0;
        for (int i = 0; i < strlen(str); i++) {
            int charNum = str[i] - '0';
            if (charNum >= 0 && charNum <= 9) {
                digit1 = charNum;
                break;
            }
        }
        for (int i = strlen(str) - 1; i >= 0; i--) {
            int charNum = str[i] - '0';
            if (charNum >= 0 && charNum <= 9) {
                digit2 = charNum;
                break;
            }
        }
        calibrationSum += (digit1 * 10) + digit2;

        current = current->next;
    }

    printf("Part 2 Calibration Sum = %d\n", calibrationSum);
}

int main(int argc, char *argv[]) {
    linkedList_s ll = getInputFile("assets/2023/Day1.txt");
    // linkedList_s ll = getInputFile("assets/test.txt");
    // printSNodeList(LLHead);

    part1(ll);
    part2(ll);

    return 0;
}
