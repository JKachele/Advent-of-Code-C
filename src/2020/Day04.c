/*************************************************
 *File----------Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Feb 25, 2026 17:59:25 UTC
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

typedef struct passport {
        int numFields;
        int byr;
        int iyr;
        int eyr;
        char *hgt;
        char *hcl;
        char *ecl;
        char *pid;
        int cid;
} passport;
static const passport EmptyPassport = {0, -1, -1, -1, NULL, NULL, NULL, NULL, -1};
typedef tll(passport) tllpassport;

struct input {
        tllpassport passports;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

bool allFieldsPresent(passport p) {
        if (p.numFields == 8)   // Has all fields
                return true;
        if (p.numFields == 7) { // Has 1 field missing
                // Missing field must be CID
                return p.cid == -1;
        }
        // Morn than 1 field missing is always invalid
        return false;
}

bool validHeight(char *hgt) {
        int num = strtol(strtok(hgt, "ci"), NULL, 10);
        char *unit = strtok(NULL, "");

        if (unit == NULL)
                return false;
        if (unit[0] == 'm' && (num >= 150 && num <= 193))
                return true;
        if (unit[0] == 'n' && (num >= 59 && num <= 76))
                return true;

        return false;
}

bool validHairColor(char *hcl) {
        if (hcl[0] != '#' || strlen(hcl) != 7)
                return false;

        for (int i = 1; i < 7; i++) {
                if (strchr("0123456789abcdef", hcl[i]) == NULL)
                        return false;
        }
        return true;
}

bool validEyeColor(char *ecl) {
        const char *Colors[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

        if (strlen(ecl) != 3)
                return false;

        for (int i = 0; i < 7; i++) {
                if (strcmp(ecl, Colors[i]) == 0)
                        return true;
        }
        return false;
}

bool validPID(char *pid) {
        if (strlen(pid) != 9)
                return false;

        for (int i = 0; i < 9; i++) {
                if (strchr("0123456789", pid[i]) == NULL)
                        return false;
        }
        return true;
}

bool allFieldsValid(passport p) {
        // Birth Year
        if (p.byr < 1920 || p.byr > 2002)
                return false;

        // Issue Year
        if (p.iyr < 2010 || p.iyr > 2020)
                return false;

        // Expiration Year
        if (p.eyr < 2020 || p.eyr > 2030)
                return false;

        // Height
        if (!validHeight(p.hgt))
                return false;

        // Hair Color
        if (!validHairColor(p.hcl))
                return false;

        // Eye Color
        if (!validEyeColor(p.ecl))
                return false;

        // Passport ID
        if (!validPID(p.pid))
                return false;

        return true;
}

void part1(struct input input) {
        tllpassport passports = input.passports;

        int numValid = 0;
        tll_foreach(passports, it) {
                if (allFieldsPresent(it->item))
                        numValid++;
        }

        printf("Part 1: %d\n\n", numValid);
}

void part2(struct input input) {
        tllpassport passports = input.passports;

        int numValid = 0;
        tll_foreach(passports, it) {
                passport p = it->item;
                if (allFieldsPresent(p) && allFieldsValid(p)) {
                        numValid++;
                }
        }

        printf("Part 2: %d\n\n", numValid);
}

void parsePassport(char *str, passport *p) {
        char *key = strtok(str, ":");
        while (key != NULL) {
                char *value = strtok(NULL, " ");
                p->numFields++;

                if (strcmp(key, "byr") == 0) {
                        p->byr = strtol(value, NULL, 10);
                } else if (strcmp(key, "iyr") == 0) {
                        p->iyr = strtol(value, NULL, 10);
                } else if (strcmp(key, "eyr") == 0) {
                        p->eyr = strtol(value, NULL, 10);
                } else if (strcmp(key, "hgt") == 0) {
                        p->hgt = malloc(strlen(value) + 1);
                        strncpy(p->hgt, value, strlen(value) + 1);
                } else if (strcmp(key, "hcl") == 0) {
                        p->hcl = malloc(strlen(value) + 1);
                        strncpy(p->hcl, value, strlen(value) + 1);
                } else if (strcmp(key, "ecl") == 0) {
                        p->ecl = malloc(strlen(value) + 1);
                        strncpy(p->ecl, value, strlen(value) + 1);
                } else if (strcmp(key, "pid") == 0) {
                        p->pid = malloc(strlen(value) + 1);
                        strncpy(p->pid, value, strlen(value) + 1);
                } else if (strcmp(key, "cid") == 0) {
                        p->cid = strtol(value, NULL, 10);
                } else {
                        printf("ERROR: Key not recognized: %s\n", key);
                }

                key = strtok(NULL, ":");
        }
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        tllpassport passports = tll_init();

        passport curPassport = EmptyPassport;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                if (strlen(str) == 0) {
                        tll_push_back(passports, curPassport);
                        curPassport = EmptyPassport;
                        continue;
                }
                parsePassport(str, &curPassport);
        }
        tll_push_back(passports, curPassport);

        input.passports = passports;
        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day04.txt";
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

