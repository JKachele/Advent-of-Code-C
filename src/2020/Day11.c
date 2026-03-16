/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 03, 2026 17:41:26 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
// #include "../lib/tllist.h"
#include "../util/vector.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef struct seat {
        bool isSeat;
        bool taken;
        int numTaken;
        int numTakenNew;
        ivec2 visibleSeats[8];
} seat;

struct input {
        ivec2 size;
        seat **seats;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printSeats(ivec2 size, seat seats[][size.x]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (!seats[y][x].isSeat)
                                printf(".");
                        else if (seats[y][x].taken)
                                printf("#");
                        else
                                printf("L");
                }
                printf("\n");
        }
        printf("\n");
}

bool validPos(int x, int y, ivec2 size) {
        return x >= 0 && x < size.x && y >= 0 && y < size.y;
}

void updateVisible(int x, int y, ivec2 size, seat seats[][size.x], bool add) {
        for (int i = 0; i < 8; i++) {
                ivec2 visPos = seats[y][x].visibleSeats[i];
                seat *visSeat = &seats[visPos.y][visPos.x];
                if (!visSeat->isSeat) continue;

                visSeat->numTakenNew += add ? 1 : -1;
        }
}

void updateAdjacent(int x, int y, ivec2 size, seat seats[][size.x], bool add) {
        for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                        if (dy == 0 && dx == 0) continue;

                        int newY = y + dy;
                        int newX = x + dx;
                        if (newY < 0 || newY >= size.y) continue;
                        if (newX < 0 || newX >= size.x) continue;

                        seats[newY][newX].numTakenNew += add ? 1 : -1;
                }
        }
}

int updateSeats(ivec2 size, seat seats[][size.x], bool part1) {
        int takenLimit = part1 ? 4 : 5;

        int numChanges = 0;

        // Update seats. if seat was changed, update adjacent setas new counters
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        seat *cur = &seats[y][x];
                        if (!cur->isSeat) continue;

                        // Create empty seat
                        if (cur->taken && cur->numTaken >= takenLimit) {
                                if (part1)
                                        updateAdjacent(x, y, size, seats, false);
                                else
                                        updateVisible(x, y, size, seats, false);
                                cur->taken = false;
                                numChanges++;
                        } else if (!cur->taken && cur->numTaken == 0) {
                                if (part1)
                                        updateAdjacent(x, y, size, seats, true);
                                else
                                        updateVisible(x, y, size, seats, true);
                                cur->taken = true;
                                numChanges++;
                        }
                }
        }

        // Move new counters to the used counters
        int takenSeats = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        seats[y][x].numTaken = seats[y][x].numTakenNew;
                        if (seats[y][x].isSeat && seats[y][x].taken)
                                takenSeats++;
                }
        }

        return (numChanges == 0) ? takenSeats : -1;
}

// Can make a list of visible seats since seat positions don't change
void setVisibleSeats(int x, int y, ivec2 size, seat seats[][size.x]) {
        const ivec2 dirs[] = {{{0, -1}}, {{1, -1}}, {{1, 0}}, {{1, 1}},
                                {{0, 1}}, {{-1, 1}}, {{-1, 0}}, {{-1, -1}}};

        seat *curSeat = &seats[y][x];
        for (int i = 0; i < 8; i++) {
                curSeat->visibleSeats[i] = (ivec2){{-1, -1}};

                ivec2 curPos = {{x + dirs[i].x, y + dirs[i].y}};
                while (validPos(curPos.x, curPos.y, size)) {
                        if (seats[curPos.y][curPos.x].isSeat) {
                                curSeat->visibleSeats[i] = curPos;
                                break;
                        }

                        curPos = ivec2Add(curPos, dirs[i]);
                }
        }
}

void part1(struct input input) {
        ivec2 size = input.size;
        seat seats[size.y][size.x];
        memcpy(seats, input.seats, size.x * size.y * sizeof(seat));

        int takenSeats = 0;
        do {
                takenSeats = updateSeats(size, seats, true);
        } while (takenSeats == -1);

        printf("Part 1: %d\n\n", takenSeats);
}

void part2(struct input input) {
        ivec2 size = input.size;
        seat seats[size.y][size.x];
        memcpy(seats, input.seats, size.x * size.y * sizeof(seat));

        // Reset seats and set visible seats
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        seat *cur = &seats[y][x];
                        cur->numTaken    = 0;
                        cur->numTakenNew = 0;
                        cur->taken = false;
                        if (cur->isSeat)
                                setVisibleSeats(x, y, size, seats);
                }
        }

        int takenSeats = 0;
        do {
                takenSeats = updateSeats(size, seats, false);
        } while (takenSeats == -1);

        printf("Part 2: %d\n", takenSeats);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.seats = calloc(size.x * size.y, sizeof(seat));
        seat (*seats)[size.x] = (seat(*)[size.x])input.seats;

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                for (int x = 0; x < size.x; x++) {
                        if (str[x] == '.') {
                                seats[y][x].isSeat = false;
                        } else {
                                seats[y][x].isSeat = true;
                                seats[y][x].taken = false;
                        }
                }

                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day11.txt";
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

