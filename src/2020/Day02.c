/*************************************************
 *File----------Day02.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 25, 2026 17:25:39 UTC
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

typedef struct passwdLine {
        int num1;
        int num2;
        char c;
        char *passwd;
} passwdLine;

struct input {
        int numLines;
        passwdLine *lines;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool isValid(passwdLine line) {
        // Count number of needed char
        int numChars = 0;
        for (int i = 0; i < (int)strlen(line.passwd); i++) {
                if (line.passwd[i] == line.c)
                        numChars++;
        }

        return (numChars >= line.num1 && numChars <= line.num2);
}

bool isValid2(passwdLine line) {
        int len = strlen(line.passwd);

        int numChars = 0;
        if (line.num1 <= len && line.passwd[line.num1 - 1] == line.c)
                numChars++;
        if (line.num2 <= len && line.passwd[line.num2 - 1] == line.c)
                numChars++;

        return numChars == 1;
}

void part1(struct input input) {
        int32 numLines = input.numLines;
        passwdLine *lines = input.lines;

        int numValid = 0;
        for (int i = 0; i < numLines; i++) {
                if (isValid(lines[i]))
                        numValid++;
        }

        printf("Part 1: %d\n\n", numValid);
}

void part2(struct input input) {
        int32 numLines = input.numLines;
        passwdLine *lines = input.lines;

        int numValid = 0;
        for (int i = 0; i < numLines; i++) {
                if (isValid2(lines[i]))
                        numValid++;
        }

        printf("Part 2: %d\n\n", numValid);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int32 numLines = ll->length;
        input.numLines = numLines;
        input.lines = calloc(numLines, sizeof(passwdLine));
        passwdLine *lines = input.lines;

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                passwdLine *line = &lines[i];

                line->num1 = strtol(strtok(str, "-"), NULL, 10);
                line->num2 = strtol(strtok(NULL, " "), NULL, 10);
                line->c = strtok(NULL, ":")[0];
                char *passwd = strtok(NULL, "") + 1;
                line->passwd = malloc((strlen(passwd) + 1) * sizeof(char));
                strncpy(line->passwd, passwd, strlen(passwd) + 1);

                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day02.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day02.txt";
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

