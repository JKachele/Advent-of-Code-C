/*************************************************
 *File----------Day1.c
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

void replaceSubString(char *str, char *find, char *replace) {
    char output[BUFFER_SIZE];
    bool substr = false;
    int start = 0;

    // Set null byte to the end of string length to speed up function
    str[strlen(str)] = '\0';
    find[strlen(find)] = '\0';
    replace[strlen(replace)] = '\0';

    do {
        // Check for substring
        int j = 0;
        for (int i = 0; i < strlen(str); i++) {
            if (str[i] == find[j]) {
                if (!substr) {
                    start = i;
                    substr = true;
                }
                j++;
                if (find[j] == '\0') {
                    break;
                }
            } else if (str[i] == find[0]){
                if (!substr) {
                    start = i;
                    substr = true;
                }
                j = 1;
                if (find[j] == '\0') {
                    break;
                }
            } else {
                substr = false;
                start = 0;
                j = 0;
            }
        }

        // If substring was found, replace, leaving the first and last chars; 
        if (find[j] == '\0' && substr) {
            int i;
            // add the beginning of the string
            for (i = 0; i <= start; i++) {
                output[i] = str[i];
            }
            // add the replace string
            for (j = 0; j < strlen(replace); j++) {
                output[i] = replace[j];
                i++;
            }
            //add the remaining string
            for (j = start + strlen(find) - 1; j < strlen(str); j++) {
                output[i] = str[j];
                i++;
            }
            output[i] = '\0';
            strncpy(str, output, 256);
        } else {
            substr = false;
        }
    } while (substr);
}

void part1(llist *ll) {
    int calibrationSum = 0;
    llNode *current = ll->head;
    while(current != NULL) {
        char *str = current->str;
        int digit1 = 0;
        int digit2 = 0;
        // Search from front to back for number, stopping at first number found
        for (int i = 0; i < strlen(str); i++) {
            int charNum = str[i] - '0';
            if (charNum >= 0 && charNum <= 9) {
                digit1 = charNum;
                break;
            }
        }
        // Search from back to front for number, stopping at first number found
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

void part2(llist *ll) {
    char numString[10][BUFFER_SIZE] = {"zero", "one", "two", "three", "four",
        "five", "six", "seven", "eight", "nine"};
    char nums[10][BUFFER_SIZE] = {"0", "1", "2", "3", "4",
        "5", "6", "7", "8", "9"};
    int calibrationSum = 0;
    llNode *current = ll->head;
    while(current != NULL) {
        char *str = current->str;
        //replace all number strings with numbers
        for (int i = 0; i < 10; i++) {
            replaceSubString(str, numString[i], nums[i]);
        }

        int digit1 = 0;
        int digit2 = 0;
        // Search from front to back for number, stopping at first number found
        for (int i = 0; i < strlen(str); i++) {
            int charNum = str[i] - '0';
            if (charNum >= 0 && charNum <= 9) {
                digit1 = charNum;
                break;
            }
        }
        // Search from back to front for number, stopping at first number found
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
    llist *ll = getInputFile("assets/2023/Day1.txt");
    // llist *ll = getInputFile("assets/test.txt");
    // llist_print(ll);

    part1(ll);
    part2(ll);

    return 0;
}
