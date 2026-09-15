/*************************************************
 *File----------Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Sep 15, 2026 16:50:03 EDT
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

typedef tll(int) tllint;

struct shift {
        int guard;
        u8 asleep[60];
};

struct guard {
        bool valid;
        u8 asleep[60];
        int timeAsleep;
        int maxSleepMinute;
};

struct input {
        struct guard guards[10000];
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

int getDate(int month, int day, int hour) {
        const int MonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // If hour is 23, shift is for the next day
        if (hour == 23) {
                if (day == MonthDays[month]) {
                        month++;
                        day = 1;
                } else {
                        day++;
                }
        }

        return (month * 100) + day;
}

void part1(struct input *input) {
        struct guard *guards = input->guards;

        // Get guard with most sleep
        int maxSleep = 0;
        int sleepyGuard = 0;
        for (int i = 0; i < 10000; i++) {
                if (!guards[i].valid) continue;

                if (guards[i].timeAsleep > maxSleep) {
                        maxSleep = guards[i].timeAsleep;
                        sleepyGuard = i;
                }
        }
        struct guard sleepy = guards[sleepyGuard];

        // Get time when guard is most asleep
        int maxAsleep = 0;
        int asleepMinute = 0;
        for (int i = 0; i < 60; i++) {
                if (sleepy.asleep[i] > maxAsleep) {
                        maxAsleep = sleepy.asleep[i];
                        asleepMinute = i;
                }
        }

        int ans = sleepyGuard * asleepMinute;

        printf("Part 1: Sleepy Guard ID * Minute: %d\n\n", ans);
}

void part2(struct input *input) {
        struct guard *guards = input->guards;

        // Get minute most asleep for all guards
        for (int i = 0; i < 10000; i++) {
                if (!guards[i].valid) continue;

                struct guard *guard = &guards[i];

                int maxAsleep = 0;
                int maxSleepMinute = 0;
                for (int j = 0; j < 60; j++) {
                        if (guard->asleep[j] > maxAsleep) {
                                maxAsleep = guard->asleep[j];
                                maxSleepMinute = j;
                        }
                }
                guard->maxSleepMinute = maxSleepMinute;
        }

        // Find guard with largest sleep minute
        int maxSleep = 0;
        int maxSleepGuard = 0;
        int maxSleepMinute = 0;
        for (int i = 0; i < 10000; i++) {
                if (!guards[i].valid) continue;

                struct guard guard = guards[i];
                if (guard.asleep[guard.maxSleepMinute] > maxSleep) {
                        maxSleep = guard.asleep[guard.maxSleepMinute];
                        maxSleepGuard = i;
                        maxSleepMinute = guard.maxSleepMinute;
                }
        }

        int ans = maxSleepGuard * maxSleepMinute;

        printf("Part 2: Sleepy Guard ID * Minute: %d\n", ans);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        struct shift shifts[10000]; // Index is date of shift (MMDD)
        memset(shifts, 0, sizeof(struct shift) * 10000);
        tllint days;

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                // Get date
                char *tok = strtok(str, "-");
                int month  = strtol(strtok(NULL, "-"), NULL, 10);
                int day    = strtol(strtok(NULL, " "), NULL, 10);
                int hour   = strtol(strtok(NULL, ":"), NULL, 10);
                int minute = strtol(strtok(NULL, "]"), NULL, 10);

                int date = getDate(month, day, hour);

                // Get note (Can determine based off first char)
                char *note = strtok(NULL, "#") + 1;
                if (*note == 'G') {             // Guard starts shift
                        int guard = strtol(strtok(NULL, " "), NULL, 10);
                        shifts[date].guard = guard;
                        tll_push_back(days, date);
                } else if (*note == 'f') {      // Guard falls asleep
                        shifts[date].asleep[minute] = 2;
                } else {                        // Guard wakes up
                        shifts[date].asleep[minute] = 3;
                }
        }

        struct guard *guards = input.guards;
        tll_foreach(days, it) {
                int day = it->item;
                struct shift shift = shifts[day];

                int guard = shift.guard;
                guards[guard].valid = true;

                bool asleep = false;
                for (int i = 0; i < 60; i++) {
                        if (shift.asleep[i] == 2)
                                asleep = true;
                        if (shift.asleep[i] == 3)
                                asleep = false;
                        
                        if (asleep) {
                                guards[guard].asleep[i]++;
                                guards[guard].timeAsleep++;
                        }
                }
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day04.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day04.txt";
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

