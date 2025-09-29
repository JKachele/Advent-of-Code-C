/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 29, 2025 20:05:25 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

struct input {
        int p1;
        int p2;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printCases(int64 cases[][10]) {
        printf("        ");
        for (int i=0; i<10; i++)
                printf("%7d ", i+1);
        printf("\n");
        for (int i=0; i<=20; i++) {
                printf("%7d ", i);
                for (int j=0; j<10; j++) {
                        if (cases[i][j] != 0)
                                printf("%7ld ", cases[i][j]);
                        else
                                printf("      - ");
                }
                printf("\n");
        }
        printf("\n");
}

void copyCases(int64 src[][10], int64 dest[][10]) {
        for (int i=0; i<=20; i++) {
                for (int j=0; j<10; j++) {
                        dest[i][j] = src[i][j];
                        src[i][j] = 0;
                }
        }
}

// Returns num dice that reach 21
int64 rollDice(int64 curState[][10], int64 cases[][10], int roll) {
        // Number of posibilities to get dice roll
        const int Rolls[10] = {0, 0, 0, 1, 3, 6, 7, 6, 3, 1};

        int64 numFinished = 0;
        for (int i=0; i<10; i++) {
                int space = ((i + roll) % 10);
                for (int j=20; j>=0; j--) {
                        if ((j + space + 1) >= 21) {
                                numFinished += (curState[j][i] * Rolls[roll]);
                                continue;
                        }
                        int score = j + space + 1;
                        cases[score][space] += (curState[j][i] * Rolls[roll]);
                }
        }
        return numFinished;
}

void getPathsTo21(int startPos, lvec2 *finishes) {
        // Possible scores are 0 - 20
        // Possible spases are 1 - 10 (0 - 9)
        int64 cases[21][10] = {0};

        // Starting Position
        cases[0][startPos-1] = 1;

        // Max of 21 rolls to finish
        for (int i=1; i<=21; i++) {
                int64 curState[21][10] = {0};
                copyCases(cases, curState);
                // Possible rolls are 3 - 9 (all 1s to all 3s)
                for (int j=3; j<=9; j++) {
                        finishes[i].x += rollDice(curState, cases, j);
                }
                
                // Count number of non-finished paths
                for (int j=0; j<=20; j++) {
                        for (int k=0; k<10; k++) {
                                finishes[i].y += cases[j][k];
                        }
                }

                // printf("After %d rolls:\n", i);
                // printf("\tFinished: %ld\n", finishes[i].x);
                // printf("\tUnfinished: %ld\n", finishes[i].y);
                // printCases(cases);
        }

        // for (int i=1; i<=21; i++) {
        //         printf("%d: %ld\n", i, finishes[i].y);
        // }
}

void part1(struct input input) {
        // Sub 1 to go from 1 index to 0 index
        int pos[2] = {input.p1 - 1, input.p2 - 1};
        int score[2] = {0, 0};
        int dice = 0;
        int rolls = 0;

        while (score[0] < 1000 && score[1] < 1000) {
                int p = (rolls % 2);
                int roll = 0;
                for (int i=0; i<3; i++) {
                        roll += dice + 1;
                        dice = (dice + 1) % 100;
                        rolls++;
                }

                pos[p] = (pos[p] + roll) % 10;
                score[p] += pos[p] + 1;
        }
        int loosingScore = score[0] < score[1] ? score[0] : score[1];
        int64 ans = loosingScore * rolls;

        printf("Part 1: %ld\n\n", ans);
}

void part2(struct input input) {
        lvec2 *finishes1 = calloc(22, sizeof(lvec2));
        lvec2 *finishes2 = calloc(22, sizeof(lvec2));
        getPathsTo21(input.p1, finishes1);
        getPathsTo21(input.p2, finishes2);

        int64 p1Wins = 0;
        int64 p2Wins = 0;
        for (int i=1; i<=21; i++) {
                // P1 plays first
                p1Wins += finishes1[i].x * finishes2[i-1].y;
                p2Wins += finishes2[i].x * finishes1[i].y;
        }
        int64 winner = p1Wins > p2Wins ? p1Wins : p2Wins;

        printf("Part 2: %ld\n", winner);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str1 = (char*)ll->head->data;
        char *str2 = (char*)ll->tail->data;
        input.p1 = strtol(str1+28, NULL, 10);
        input.p2 = strtol(str2+28, NULL, 10);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        clock_t parse = clock();
        part1(input);
        clock_t pt1 = clock();
        part2(input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

