/*************************************************
 *File----------Day4.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Mar 28, 2024 07:57:33 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

// Part 1 {{{
void part1(llist *ll) {
        llNode *current = ll->head;
        int totalPoints = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (str[0] == '\0') break;

                strtok(str, ":");   // Remove First Part of string
                char *winNumbersStr = strtok(NULL, "|");    // Split Str at '|'
                char *myNumbersStr = strtok(NULL, "");
                int numWinNumbers = (strlen(winNumbersStr) - 1) / 3;
                int numMyNumbers = strlen(myNumbersStr) / 3;

                // Get array of winning Numbers
                int winNumbers[numWinNumbers];
                char *num = strtok(winNumbersStr, " ");
                for (int i = 0; i < numWinNumbers; i++) {
                        winNumbers[i] = strtol(num, (char**)NULL, 10);
                        num = strtok(NULL, " ");
                }

                // Get "Hash" table of ny numbers
                // index is value, element is if it is one of the numbers
                bool myNumbers[100] = {false};
                num = strtok(myNumbersStr, " ");
                for (int i = 0; i < numMyNumbers; i++) {
                        char *end;
                        int number = strtol(num, (char**)NULL, 10);
                        myNumbers[number] = true;
                        num = strtok(NULL, " ");
                }

                int winCount = 0;
                for (int i = 0; i < numWinNumbers; i++) {
                        if (myNumbers[winNumbers[i]]) {
                                winCount++;
                        }
                }

                if (winCount > 0) {
                        // Powers of 2 can be accomplished with a bit shift.
                        // Every bit shift multiplies my 2.
                        // Subtract 1 from numCount because 1 win is only 1 point which is 2^0
                        int points = 1 << (winCount - 1);
                        totalPoints += points;
                }

                current = current->next;
        }
        printf("Part 1: Total Points: %d\n", totalPoints);
}   // }}}

// Part 2 {{{
void part2(llist *ll) {
        llNode *current = ll->head;
        int cardCount[ll->length];
        for (int i = 0; i < ll->length; i++) { cardCount[i] = 1; }
        int cardNum = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                if (str[0] == '\0') break;

                strtok(str, ":");   // Remove First Part of string
                char *winNumbersStr = strtok(NULL, "|");    // Split Str at '|'
                char *myNumbersStr = strtok(NULL, "");
                int numWinNumbers = (strlen(winNumbersStr) - 1) / 3;
                int numMyNumbers = strlen(myNumbersStr) / 3;

                // Get array of winning Numbers
                int winNumbers[numWinNumbers];
                char *num = strtok(winNumbersStr, " ");
                for (int i = 0; i < numWinNumbers; i++) {
                        winNumbers[i] = strtol(num, (char**)NULL, 10);
                        num = strtok(NULL, " ");
                }

                // Get "Hash" table of ny numbers
                // index is value, element is if it is one of the numbers
                bool myNumbers[100] = {false};
                num = strtok(myNumbersStr, " ");
                for (int i = 0; i < numMyNumbers; i++) {
                        char *end;
                        int number = strtol(num, (char**)NULL, 10);
                        myNumbers[number] = true;
                        num = strtok(NULL, " ");
                }

                int winCount = 0;
                for (int i = 0; i < numWinNumbers; i++) {
                        if (myNumbers[winNumbers[i]]) {
                                winCount++;
                        }
                }

                for (int i = 1; i <= winCount; i++) {
                        cardCount[cardNum + i] += (1 * cardCount[cardNum]);
                } 

                current = current->next;
                cardNum++;
        }

        int totalCards = 0;
        for (int i = 0; i < ll->length; i++) {
                totalCards += cardCount[i];
        }

        printf("Part 2: Total Card Count: %d\n", totalCards);
}   // }}}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2023/Day4.txt");
        } else {
                ll = getInputFile("assets/inputs/2023/Day4.txt");
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
