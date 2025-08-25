/*************************************************
 *File----------Day25.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Aug 25, 2025 22:01:59 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int64 snafuToInt(char *snafu) {
        int32 len = strlen(snafu);
        // printf("%d: %s\n", len, snafu);

        int64 out = 0;
        for (int i=0; i<len; i++) {
                out *= 5;
                char c = snafu[i];
                switch(c) {
                case '2':
                        out += 2;
                        break;
                case '1':
                        out += 1;
                        break;
                case '0':
                        break;
                case '-':
                        out -= 1;
                        break;
                case '=':
                        out -= 2;
                        break;
                default:
                        printf("Invalid character '%c' in input\n", c);
                        break;
                }
        }
        // printf("%s: %d\n", snafu, out);

        return out;
}

void intToSnafu(int64 num, char *snafu) {
        char snafuDigits[5] = {'0', '1', '2', '=', '-'};

        if (num == 0) {
                snafu[0] = '\0';
                return;
        }

        char snafuDigit = snafuDigits[num % 5];

        int64 newNum = (num + 2) / 5; // +2 because snafu numbers start at -2
        intToSnafu(newNum, snafu);
        
        // Add the snafu digit to the end
        int32 len = strlen(snafu);
        snafu[len] = snafuDigit;
        snafu[len + 1] = '\0';
}

void part1(llist *ll) {
        int64 fuelTotal = 0;
        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                fuelTotal += snafuToInt(str);

                current = current->next;
        }
        printf("%ld\n", fuelTotal);
        char snafu[100] = {0};
        intToSnafu(fuelTotal, snafu);
        printf("%ld\n", snafuToInt(snafu));

        printf("Part 1: %ld -> %s\n\n", fuelTotal, snafu);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2022/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2022/Day25.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Solve: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

