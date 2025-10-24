/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Oct 06, 2025 20:47:45 UTC
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
#include "../util/talist.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096
#define HALLWAY_LENGTH 11
#define NUM_ROOMS 4
#define ROOM_SIZE 2

typedef struct hashstate {
        u64 state;
        u64 cost;
} hashstate;
typedef tal(hashstate) talhashstate;

// Empty: 0, ABCD: 1-4
typedef struct state {
        u8 hallway[HALLWAY_LENGTH];
        u8 rooms[NUM_ROOMS][ROOM_SIZE];
} state;

struct input {
        state initState;
};

static int32 COST[] = {0, 1, 10, 100, 1000};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printState(state s) {
        for (int i = 0; i < HALLWAY_LENGTH; i++) {
                if (s.hallway[i] == 0)
                        printf("0");
                else
                        printf("%c", 'A' + s.hallway[i] - 1);
        }
        printf("\n");
        for (int i = 0; i < ROOM_SIZE; i++) {
                printf("  ");
                for (int j = 0; j < NUM_ROOMS; j++) {
                        if (s.rooms[j][i] == 0)
                                printf("0 ");
                        else
                                printf("%c ", 'A' + s.rooms[j][i] - 1);
                }
                printf("\n");
        }
        printf("\n");
}

u64 statetohash(state s) {
        u64 hash = 0;
        for (int i = 0; i < HALLWAY_LENGTH; i++) {
                hash *= 5;
                hash += s.hallway[i];
        }
        for (int i = 0; i < NUM_ROOMS; i++) {
                for (int j = 0; j < ROOM_SIZE; j++) {
                        hash *= 5;
                        hash += s.rooms[i][j];
                }
        }
        return hash;
}

state hashtostate(u64 hash) {
        state s = {0};
        for (int i=NUM_ROOMS-1; i>=0; i--) {
                for (int j=ROOM_SIZE-1; j>=0; j--) {
                        s.rooms[i][j] = hash % 5;
                        hash /= 5;
                }
        }
        for (int i=HALLWAY_LENGTH-1; i>=0; i--) {
                s.hallway[i] = hash % 5;
                hash /= 5;
        }
        return s;
}

// Visited gets ordered by hash. Returns true if state is added or modified
bool insertVisited(talhashstate *list, hashstate s) {
        // If list is empty, add state
        if (list->length == 0) {
                tal_add(*list, s);
                return true;
        }

        // Binary Search for hash: low will have the final index
        u64 sValue = s.state;
        int low = 0;
        int high = list->length - 1;
        bool found = false;
        while (low <= high) {
                int mid = low + (high - low) / 2;
                u64 curValue = list->array[mid].state;

                if (curValue == sValue) {
                        found = true;
                        low = mid;
                        break;
                }

                if (curValue < sValue)
                        low = mid + 1;
                else
                        high = mid - 1;
        }
        if (found) {
                if (list->array[low].cost > s.cost) {
                        list->array[low].cost = s.cost;
                        return true;
                }
                return false;
        }
        tal_insert(*list, low, s);
        return true;
}

// Queue gets ordered by cost
void insertQueue(talhashstate *list, hashstate s) {
        // If list is empty, add state
        if (list->length == 0) {
                tal_add(*list, s);
                return;
        }

        // Linear search for hash
        for (int i=0; i<list->length; i++) {
                if (list->array[i].state == s.state) {
                        hashstate *curHS = &list->array[i];
                        // If state already in queue with same or lower cost
                        // Ignore new state
                        if (curHS->cost <= s.cost)
                                return;
                        // Else remove old state and insert new state
                        tal_remove(*list, i);
                        break;
                }
        }

        // Binary Search for cost: low will have the final index
        u64 sValue = s.cost;
        int low = 0;
        int high = list->length - 1;
        while (low <= high) {
                int mid = low + (high - low) / 2;
                u64 curValue = list->array[mid].cost;

                if (curValue == sValue) {
                        low = mid;
                        break;
                }

                if (curValue < sValue)
                        low = mid + 1;
                else
                        high = mid - 1;
        }
        tal_insert(*list, low, s);
}

bool isRoomEnterable(state s, int room) {
        for (int i=0; i<ROOM_SIZE; i++) {
                // If room contains an amphipod that does not belong there
                if (s.rooms[room][i] != 0 && s.rooms[room][i] != room + 1)
                        return false;
        }
        return true;
}

talhashstate getMoves(hashstate hs) {
        talhashstate moves = tal_init();
        state s = hashtostate(hs.state);

        return moves;
}

u32 dijkstra(state init) {
        talhashstate visited = tal_init();
        talhashstate queue = tal_init();

        u64 initHash = statetohash(init);
        hashstate initHS = {initHash, 0};
        tal_add(visited, initHS);
        tal_add(queue, initHS);

        while (queue.length > 0) {
                hashstate curS = tal_pop_front(queue);

                talhashstate moves = getMoves(curS);
                for (int i=0; i<moves.length; i++) {
                        if (insertVisited(&visited, moves.array[i])) {
                                insertQueue(&queue, moves.array[i]);
                        }
                }
        }

        return 0;
}

void part1(struct input input) {
        state initState = input.initState;
        // printState(initState);

        printf("Part 1: \n\n");
}

void part2(struct input input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        state *initState = &input.initState;

        // Top of rooms is 3rd line
        llNode *current = ll->head->next->next;
        int line = 0;
        while(current != NULL) {
                char *str = (char*)current->data;
                for (int i=0; i<4; i++) {
                        // Room indexes are 3, 5, 7, and 9
                        int strIndex = (i * 2) + 3;
                        initState->rooms[i][line] = (str[strIndex] - 'A') + 1;
                }
                line++;
                if (line > 1)
                        break;
                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day23.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day23.txt";
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

