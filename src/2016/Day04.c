/*************************************************
 *File----------Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Oct 27, 2026 10:09:00 EDT
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

struct room {
        bool valid;
        char *name;
        int id;
        char check[6];
};

struct input {
        int numRooms;
        struct room *rooms;
};

void decryptName(struct room room) {
        for (int i = 0; i < (int)strlen(room.name); i++) {
                char c = room.name[i];
                if (c == '-') {
                        room.name[i] = ' ';
                        continue;
                }

                int cint = c - 'a';
                cint += room.id;
                cint %= 26;
                room.name[i] = cint + 'a';
        }
}

void part1(struct input *input) {
        int numRooms = input->numRooms;
        struct room *rooms = input->rooms;

        int realSum = 0;
        for (int i = 0; i < numRooms; i++) {
                struct room room = rooms[i];
                // printf("%s, %d, %s\n", room.name, room.id, room.check);

                // Get count of letters in name
                int letterCount[26] = {0};
                for (int j = 0; j < (int)strlen(room.name); j++) {
                        if (room.name[j] == '-') { continue; }
                        int c = room.name[j] - 'a';
                        letterCount[c]++;
                }

                // Calculate checksum. Most used letters in order, ties broken by alphabetization
                int top5Count[5] = {-1, -1, -1, -1, -1};
                int top5[5] = {0, 1, 2, 3, 4};
                for (int j = 0; j < 26; j++) {
                        for (int k = 0; k < 5; k++) {
                                if (letterCount[j] < top5Count[k]) { continue; }
                                if (letterCount[j] == top5Count[k] && j > top5[k]) { continue; }
                                for (int l = 4; l > k; l--) {
                                        top5Count[l] = top5Count[l-1];
                                        top5[l] = top5[l-1];
                                }
                                top5Count[k] = letterCount[j];
                                top5[k] = j;
                                break;
                        }
                }

                // Verify Checksum
                bool real = true;
                for (int j = 0; j < 5; j++) {
                        if ((room.check[j] - 'a') != top5[j]) {
                                real = false;
                                break;
                        }
                }
                if (real) {
                        realSum += room.id;
                        rooms[i].valid = true;
                } else {
                        rooms[i].valid = false;
                }
        }

        printf("Part 1: Real Rooms Id = %d\n\n", realSum);
}

void part2(struct input *input) {
        const char *RoomName = "northpole object storage";

        int numRooms = input->numRooms;
        struct room *rooms = input->rooms;

        int roomId = 0;
        for (int i = 0; i < numRooms; i++) {
                struct room room = rooms[i];
                if (!room.valid) { continue; }

                decryptName(room);
                if (strcmp(room.name, RoomName) == 0) {
                        roomId = room.id;
                        break;
                }
                // printf("%d: %s\n", room.id, room.name);
        }

        printf("Part 2: North Pole Storage Room = %d\n", roomId);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.numRooms = ll->length;
        input.rooms = malloc(input.numRooms * sizeof(struct room));

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                struct room room = {0};

                room.name = calloc(strlen(str), sizeof(char));
                char *tok = strtok(str, "-");
                int j = 0;
                while (tok != NULL) {
                        if (tok[0] >= '0' && tok[0] <= '9') {
                                break;
                        }
                        for (int k = 0; k < (int)strlen(tok); k++) {
                                room.name[j++] = tok[k];
                        }
                        room.name[j++] = '-';
                        tok = strtok(NULL, "-[]");
                }
                room.name[j - 1] = '\0';

                room.id = strtol(tok, NULL, 10);

                tok = strtok(NULL, "]");
                strncpy(room.check, tok, 5);

                input.rooms[i++] = room;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day04.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day04.txt";
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
        double totalTime = ((double)(pt2 - begin) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f, Total: %f\n", 
                        parseTime, pt1Time, pt2Time, totalTime);
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f, Total: %f\n", 
        //                 parseTime, pt1Time, totalTime);

        return 0;
}

