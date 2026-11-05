/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 27, 2026 12:58:42 EDT
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
#include "../util/vector.h"
#include "../util/talist.h"

#define INPUT_BUFFER_SIZE 1024

struct input {
        int numips;
        char **ips;
};

bool isABBA(char *str) {
        return (str[0] == str[3]) && (str[1] == str[2]) && (str[0] != str[1]);
}

bool isABAorBAB(char *str) {
        // Make sure it doesnt cross the square brackets
        return (str[0] == str[2]) && (str[0] != str[1]) && (str[1] != '[') && (str[1] != ']');
}

void part1(struct input *input) {
        int numips = input->numips;
        char **ips = input->ips;

        int numTLS = 0;
        for (int i = 0; i < numips; i++) {
                char *ip = ips[i];

                bool hasABBA = false;
                bool hasABBAinHSN = false;
                bool inHNS = false;
                for (int j = 0; j < (int)strlen(ip) - 3; j++) {
                        char c = ip[j];
                        if (c == '[') {
                                inHNS = true;
                                continue;
                        }
                        if (c == ']') {
                                inHNS = false;
                                continue;
                        }
                        if (isABBA(&ip[j])) {
                                if (inHNS) {
                                        hasABBAinHSN = true;
                                        break;
                                } else {
                                        hasABBA = true;
                                }
                        }
                }
                if (hasABBA && !hasABBAinHSN) {
                        numTLS++;
                }
        }

        printf("Part 1: IPs with TLS = %d\n\n", numTLS);
}

void part2(struct input *input) {
        int numips = input->numips;
        char **ips = input->ips;

        int numSSL = 0;
        for (int i = 0; i < numips; i++) {
                char *ip = ips[i];
                int len = strlen(ip);

                char aba[len][3];
                char bab[len][3];
                int numABA = 0;
                int numBAB = 0;

                bool inHNS = false;
                for (int j = 0; j < len - 2; j++) {
                        char c = ip[j];
                        if (c == '[') {
                                inHNS = true;
                                continue;
                        }
                        if (c == ']') {
                                inHNS = false;
                                continue;
                        }
                        if (isABAorBAB(&ip[j])) {
                                if (inHNS) {
                                        strncpy(bab[numBAB++], &ip[j], 3);
                                } else {
                                        strncpy(aba[numABA++], &ip[j], 3);
                                }
                        }
                }

                for (int j = 0; j < numABA; j++) {
                        bool isSSL = false;
                        for (int k = 0; k < numBAB; k++) {
                                if (aba[j][0] == bab[k][1] && aba[j][1] == bab[k][0]) {
                                        numSSL++;
                                        isSSL = true;
                                        break;
                                }
                        }
                        if (isSSL) {
                                break;
                        }
                }
        }

        printf("Part 2: IPs with SSL = %d\n", numSSL);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numips = ll->length;
        input.ips = malloc(input.numips * sizeof(char*));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                char *ip = malloc(strlen(str) + 1);
                strcpy(ip, str);
                input.ips[i++] = ip;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day07.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day07.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1(&input);
        clock_t pt1 = clock();
        part2(&input);
        clock_t end = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(end - pt1) / CLOCKS_PER_SEC) * 1000;
        double totalTime = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

