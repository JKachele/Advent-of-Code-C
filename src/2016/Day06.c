/*************************************************
 *File----------Day06.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 27, 2026 12:40:42 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024

struct input {
        int numMsgs;
        int msgLength;
        char **msgs;
};

void part1_2(struct input *input) {
        int numMsgs = input->numMsgs;
        int msgLength = input->msgLength;
        char **msgs = input->msgs;

        int numChars[msgLength][26];
        for (int i = 0; i < msgLength; i++) {
                for (int j = 0; j < 26; j++) {
                        numChars[i][j] = 0;
                }
        }

        for (int i = 0; i < numMsgs; i++) {
                for (int j = 0; j < msgLength; j++) {
                        int cint = msgs[i][j] - 'a';
                        numChars[j][cint]++;
                }
        }

        char msg1[msgLength+1];
        char msg2[msgLength+1];
        for (int i = 0; i < msgLength; i++) {
                int maxChars = 0;
                int minChars = INT32_MAX;
                for (int j = 0; j < 26; j++) {
                        if (numChars[i][j] == 0) { continue; }
                        if (numChars[i][j] > maxChars) {
                                maxChars = numChars[i][j];
                                msg1[i] = j + 'a';
                        }
                        if (numChars[i][j] < minChars) {
                                minChars = numChars[i][j];
                                msg2[i] = j + 'a';
                        }
                }
        }
        msg1[msgLength] = '\0';
        msg2[msgLength] = '\0';

        printf("Part 1: Message = %s\n\n", msg1);
        printf("Part 2: Message = %s\n", msg2);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numMsgs = ll->length;
        input.msgLength = getLongestLine(ll);
        input.msgs = malloc(input.numMsgs * sizeof(char*));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                char *msg = malloc(input.msgLength + 1);
                strcpy(msg, str);
                input.msgs[i++] = msg;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day06.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day06.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
                        parseTime, pt1Time, totalTime);

        return 0;
}

