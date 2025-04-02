/*************************************************
 *File----------Day6.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 24, 2025 16:22:53 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

bool containsRepeats4(char *str) {
        for (int i = 0; i < 3; i++) {
                for (int j = i + 1; j < 4; j++) {
                        if (str[i] == str[j])
                                return true;
                }
        }
        return false;
}

bool containsRepeats14(char *str) {
        bool chars[26] = {false};
        for (int i = 0; i < 14; i++) {
                int charIndex = str[i] - 'a';
                if (chars[charIndex]) return true;
                chars[charIndex] = true;
        }
        return false;
}

void part1(llist *ll) {
        char str[BUFFER_SIZE];
        strncpy(str, (char*)ll->head->data, BUFFER_SIZE);
        int len = strlen(str);
        if (len < 4) return;

        int i;
        for (i = 0; i <= len - 4; i++) {
                if (!containsRepeats4(str + i))
                        break;
        }
        int sopIndex = i + 4;

        printf("Part 1: Start of Packet at %d\n\n", sopIndex);
}

void part2(llist *ll) {
        char str[BUFFER_SIZE];
        strncpy(str, (char*)ll->head->data, BUFFER_SIZE);
        int len = strlen(str);
        if (len < 14) return;

        int i;
        for (i = 0; i <= len - 14; i++) {
                if (!containsRepeats14(str + i))
                        break;
        }
        int somIndex = i + 14;

        printf("Part 2: Start of Message at %d\n\n", somIndex);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day6.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day6.txt");
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

