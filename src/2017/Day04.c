/*************************************************
 *File----------Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Oct 09, 2026 10:36:52 EDT
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
#include "../util/talist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 1024

typedef tal(u32) talu32;
typedef tal(char*) talstr;
typedef tal(talstr) talstr2d;

struct input {
        talstr2d passphrases;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printPassphrases(talstr2d passphrases) {
        tal_for(passphrases, i) {
                talstr passphrase = passphrases.array[i];
                tal_for(passphrase, j) {
                        printf("%s ", passphrase.array[j]);
                }
                printf("\n");
        }
}

talstr lineToStrArray(char *str) {
        talstr line = tal_init();

        char *tok = strtok(str, " ");
        while (tok != NULL) {
                int len = strlen(tok);
                char *word = malloc(len+1);
                strcpy(word, tok);
                tal_add(line, word);

                tok = strtok(NULL, " ");
        }

        return line;
}

bool validPassphrase(talstr pass) {
        tal_for(pass, i) {
                char *str1 = pass.array[i];
                for (size_t j = 0; j < i; j++) {
                        char *str2 = pass.array[j];
                        if (strcmp(str1, str2) == 0)
                                return false;
                }
        }
        return true;
}

bool validPassphrase2(talstr pass) {
        // Count the letters used in each word

        int passphraseLetters[pass.length][26];
        tal_for(pass, i) {
                for (int j = 0; j < 26; j++) {
                        passphraseLetters[i][j] = 0;
                }

                char *word = pass.array[i];
                for (int j = 0; j < (int)strlen(word); j++) {
                        passphraseLetters[i][word[j] - 'a']++;
                }
        }

        // If bit maps match, words use the same letters
        for (int i = 0; i < (int)pass.length; i++) {
                for (int j = 0; j < i; j++) {
                        bool match = true;
                        for (int k = 0; k < 26; k++) {
                                if (passphraseLetters[i][k] != passphraseLetters[j][k]) {
                                        match = false;
                                        break;
                                }
                        }
                        if (match)
                                return false;
                }
        }
        return true;
}

void part1(struct input *input) {
        talstr2d passphrases = input->passphrases;
        // printPassphrases(passphrases);

        int numValid = 0;
        tal_for(passphrases, i) {
                if (validPassphrase(passphrases.array[i]))
                        numValid++;
        }

        printf("Part 1: Valid Passphrases = %d\n\n", numValid);
}

void part2(struct input *input) {
        talstr2d passphrases = input->passphrases;

        int numValid = 0;
        tal_for(passphrases, i) {
                if (validPassphrase2(passphrases.array[i]))
                        numValid++;
        }

        printf("Part 2: Valid Passphrases = %d\n", numValid);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        // char *str = (char*)ll->head->data;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                talstr line = lineToStrArray(str);
                tal_add(input.passphrases, line);
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day04.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day04.txt";
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

