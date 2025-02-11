/*************************************************
 *File----------Day3.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Dec 04, 2024 17:18:52 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"

const char* REGEX_STR = "mul\\([[:digit:]]+,[[:digit:]]+\\)";
const char* REGEX_STR2= "(mul\\([[:digit:]]+,[[:digit:]]+\\))|(do\\(\\))|(don't\\(\\))";
bool include = true;

char* regexp(char *str, int *endPtr, regex_t regex) {
        regmatch_t rm[1];
        if (regexec(&regex, str, 1, rm, 0) != 0) {
                // printf("No Regex Found\n");
                return NULL;
        }

        // printf("Regex Found!\n");
        *endPtr += (int)rm[0].rm_eo;

        char* match = malloc(sizeof(char) * 20);
        int matchLen = (int)(rm[0].rm_eo - rm[0].rm_so);

        strncpy(match, str + rm[0].rm_so, matchLen);
        match[matchLen] = '\0';
        return match;
}

int evalStr(char *str, regex_t regex) {
        int total = 0;
        int *endPtr = malloc(sizeof *endPtr);
        *endPtr = 0;
        char *match = regexp(str, endPtr, regex);
        const int OFFSET = 4;   // mul(

        while (match != NULL) {
                // printf("Match: <<%s>>\n", match);
                // int num1 = strtol(strtok(match + OFFSET, ","), (char**)NULL, 10);
                int num1 = strtol(strtok(match + OFFSET, ","), (char**)NULL, 10);
                int num2 = strtol(strtok(NULL, ")"), (char**)NULL, 10);
                total += num1 * num2;
                match = regexp(str + *endPtr, endPtr, regex);
        }
        // printf("\n");

        free(endPtr);
        return total;
}

int evalStr2(char *str, regex_t regex) {
        int total = 0;
        int *endPtr = malloc(sizeof *endPtr);
        *endPtr = 0;
        char *match = regexp(str, endPtr, regex);
        const int OFFSET = 4;   // mul(

        while (match != NULL) {
                if (strcmp(match, "do()") == 0) {
                        include = true;
                        goto nextMatch;
                } else if (strcmp(match, "don't()") == 0) {
                        include = false;
                        goto nextMatch;
                } else if (!include) {
                        goto nextMatch;
                }
                int num1 = strtol(strtok(match + OFFSET, ","), (char**)NULL, 10);
                int num2 = strtol(strtok(NULL, ")"), (char**)NULL, 10);
                total += num1 * num2;
        nextMatch:
                match = regexp(str + *endPtr, endPtr, regex);
        }

        free(endPtr);
        return total;
}

void part1(llist *ll) {
        llNode *current = ll->head;

        regex_t regex;
        if (regcomp(&regex, REGEX_STR, REG_EXTENDED) != 0) {
                fprintf(stderr, "Regex Compilation Error\n");
                return;
        }

        long total = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                int lineTotal = evalStr(str, regex);
                total += lineTotal;
                current = current->next;
        }
        printf("\nPart 1: Total: %ld\n", total);
}

void part2(llist *ll) {
        llNode *current = ll->head;

        regex_t regex;
        if (regcomp(&regex, REGEX_STR2, REG_EXTENDED) != 0) {
                fprintf(stderr, "Regex Compilation Error\n");
                return;
        }

        long total = 0;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                int lineTotal = evalStr2(str, regex);
                total += lineTotal;
                current = current->next;
        }
        printf("\nPart 2: Total: %ld\n", total);
}

int main(int argc, char *argv[]) {
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0)
                ll = getInputFile("assets/tests/2024/Day3.txt");
        else
                ll = getInputFile("assets/2024/Day3.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
