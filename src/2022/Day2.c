/*************************************************
 *File----------Day2.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 19, 2025 11:28:11 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

// (A,X) = Rock, (B,Y) = Paper, (C,Z) = Scissors
int8 roundScore(char oponent, char me) {
        int8 score = 0;

        // Rock = 0, Paper = 1, Scissors = 2
        int8 oponentVal = oponent - 'A';
        int8 myVal = me - 'X';

        // Formula: 0 = tie, 1 = Win, 2 = Loss
        int8 result = ((myVal - oponentVal) + 3) % 3;
        if (result == 1)
                score += 6;
        else if (result == 0)
                score += 3;

        // Add shape selected
        score += myVal + 1;

        return score;
}

int8 roundScore2(char oponent, char me) {
        int8 score = 0;

        // Rock = 0, Paper = 1, Scissors = 2
        // Outcome: -1 = loss, 0 = tie, 1 = win;
        int8 oponentVal = oponent - 'A';
        int8 outcome = me - 'Y';

        score += (outcome + 1) * 3;

        int8 myVal = ((oponentVal + outcome) + 3) % 3;
        score += myVal + 1;

        return score;
}

void part1(llist *ll) {
        int32 totalScore = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                totalScore += roundScore(str[0], str[2]);

                current = current->next;
        }
        printf("Part 1: Total Score: %d\n\n", totalScore);
}

void part2(llist *ll) {
        int32 totalScore = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                totalScore += roundScore2(str[0], str[2]);

                current = current->next;
        }
        printf("Part 2: Total Score: %d\n\n", totalScore);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day2.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day2.txt");
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

