/*************************************************
 *File----------Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 22, 2026 12:54:27 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 16384

typedef tll(char*) tllstr;

struct input {
        tllstr boxes;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int numDiff(char *str1, char *str2, int len) {
        int diff = 0;
        for (int i = 0; i < len; i++) {
                if (str1[i] != str2[i])
                        diff++;
        }
        return diff;
}

void part1(struct input *input) {
        int numTwos = 0;
        int numThrees = 0;
        tll_foreach(input->boxes, it) {
                char *str = it->item;
                int letters[26] = {0};
                for (int i = 0; i < (int)strlen(str); i++)
                        letters[str[i] - 'a']++;
                bool two = false;
                bool three = false;
                for (int i = 0; i < 26; i++) {
                        if (letters[i] == 2)
                                two = true;
                        if (letters[i] == 3)
                                three = true;
                }
                if (two) numTwos++;
                if (three) numThrees++;
        }

        int checksum = numTwos * numThrees;

        printf("Part 1: %d\n\n", checksum);
}

void part2(struct input *input) {
        int len = strlen(tll_front(input->boxes));
        char *com1;
        char *com2;
        tll_foreach(input->boxes, it) {
                char *str1 = tll_pop_front(input->boxes);
                bool found = false;
                tll_foreach(input->boxes, it2) {
                        int diff = numDiff(str1, it2->item, len);
                        if (diff == 1) {
                                found = true;
                                com1 = str1;
                                com2 = it2->item;
                                break;
                        }
                }
        }

        char *common = malloc(len);
        int j = 0;
        for (int i = 0; i < len; i++) {
                if (com1[i] == com2[i]) {
                        common[j++] = com1[i];
                }
        }
        common[len-1] = '\0';

        printf("Part 2: %s\n", common);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                char *cpy = malloc(strlen(str) + 1);
                strncpy(cpy, str, strlen(str));
                cpy[strlen(str)] = '\0';
                tll_push_back(input.boxes, cpy);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day02.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

