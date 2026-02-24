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
#define ROOM_SIZE 4

typedef struct hashstate {
        u64 state;
        u64 cost;
} hashstate;
typedef tal(hashstate) talhashstate;

// Empty: 0, ABCD: 1-4
// Hallway: 0 --> 10
// Rooms: (0, 0) (1, 0) (2, 0) (3, 0)
//        (0, 1) (1, 1) (2, 1) (3, 1)
//        (0, 2) (1, 2) (2, 2) (3, 2)
//        (0, 3) (1, 3) (2, 3) (3, 3)
typedef struct state {
        u8 hallway[HALLWAY_LENGTH];
        u8 rooms[NUM_ROOMS][ROOM_SIZE];
} state;

struct input {
        state initState;
};

static int32 COST[] = {0, 1, 10, 100, 1000};
static int32 HALLWAY_MOVES[] = {0, 1, 3, 5, 7, 9, 10};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printAPod(int n) {
        if (n == 0)
                printf(".");
        else
                printf("%c", 'A' + (n - 1));
}

void printState(state s) {
        for (int i = 0; i < HALLWAY_LENGTH; i++) {
                printAPod(s.hallway[i]);
        }
        printf("\n");
        for (int i = 0; i < ROOM_SIZE; i++) {
                printf("  ");
                for (int j = 0; j < NUM_ROOMS; j++) {
                        printAPod(s.rooms[j][i]);
                        printf(" ");
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

u64 getEndState(void) {
        state s = {0};
        for (int i = 0; i < ROOM_SIZE; i++) {
                for (int j = 0; j < NUM_ROOMS; j++) {
                        s.rooms[j][i] = j + 1;
                }
        }
        return statetohash(s);
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
        for (int i=0; i<(int)list->length; i++) {
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

bool hallwayClear(state s, int startPos, int endPos) {
        int lowerPos = startPos < endPos ? startPos : endPos;
        int higherPos = startPos < endPos ? endPos : startPos;

        for (int i = lowerPos; i <= higherPos; i++) {
                if (i == startPos) continue;

                if (s.hallway[i] != 0)
                        return false;
        }

        return true;
}

bool roomToRoom(state s, int startRoom, int targetRoom) {
        if (!isRoomEnterable(s, targetRoom))
                return false;

        if (hallwayClear(s, (startRoom + 1) * 2, (targetRoom + 1) * 2))
                return true;
        return false;
}

int addApodToRoom(state *s, int room) {
        for (int i = ROOM_SIZE - 1; i >= 0; i--) {
                if (s->rooms[room][i] == 0) {
                        s->rooms[room][i] = room + 1;
                        return i;
                }
        }
        return -1;
}

talhashstate moveFromRoom(state s, u64 cost, int room, int roomPos) {
        talhashstate moves = tal_init();
        u8 apodType = s.rooms[room][roomPos];

        // Check if able to move:
        // Is there an APod above it?
        bool ableToMove = true;
        for (int i = 0; i < roomPos; i++) {
                if (s.rooms[room][i] != 0)
                        ableToMove = false;
        }
        if (!ableToMove)
                return moves;
        // is it in its own room?
        if (room == apodType - 1) {
                // are all apods below it in the right room?
                bool ableToMove = false;
                for (int i = roomPos; i < ROOM_SIZE; i++) {
                        if (s.rooms[room][i] != apodType)
                                ableToMove = true;
                }
                if (!ableToMove)
                        return moves;
        }
        
        // Moving into its own room is always best possible choice
        if (roomToRoom(s, room, apodType - 1)) {
                int newRoom = apodType - 1;
                int newPos = 0;
                state newS = s;
                // remove apod from old room
                newS.rooms[room][roomPos] = 0;
                // add apod to new room
                newPos = addApodToRoom(&newS, newRoom);
                u64 newHash = statetohash(newS);
                // calculate cost for this move
                u64 newCost = (abs(newRoom - room) * 2) + 2; // Hallway Cost
                newCost += roomPos + newPos; // Add for movements in the rooms
                newCost *= COST[apodType];
                hashstate newHS = {newHash, cost + newCost};
                tal_add(moves, newHS);
                return moves;
        }

        // Move to hallway
        for (int i = 0; i < HALLWAY_LENGTH - 4; i++) {
                int newPos = HALLWAY_MOVES[i];
                int curPos = (room + 1) * 2; // Hallway pos above room
                // check if move is possible
                if (!hallwayClear(s, curPos, newPos))
                        continue;

                state newS = s;
                // move apod
                newS.rooms[room][roomPos] = 0;
                newS.hallway[newPos] = apodType;
                u64 newHash = statetohash(newS);
                // calculate cost for this move
                u64 newCost = (abs(newPos - curPos) + roomPos + 1) * COST[apodType];
                hashstate newHS = {newHash, cost + newCost};
                tal_add(moves, newHS);
        }

        return moves;
}

talhashstate moveFromHall(state s, u64 cost, int pos) {
        talhashstate moves = tal_init();
        u8 apodType = s.hallway[pos];

        // if room is unavilable, no moves are possible
        if (!isRoomEnterable(s, apodType - 1))
                return moves;

        // if path to room is blocked, no moves are possible
        // pos above room = (room + 1) * 2 and room = apodType - 1
        if (!hallwayClear(s, pos, apodType * 2))
                return moves;

        int newRoom = apodType - 1;
        int newPos = 0;
        state newS = s;
        // remove apod from hallway
        newS.hallway[pos] = 0;
        // add apod to new room
        newPos = addApodToRoom(&newS, newRoom);
        u64 newHash = statetohash(newS);
        // calculate cost for this move
        u64 newCost = (abs(pos - (apodType*2)) + newPos + 1) * COST[apodType];
        hashstate newHS = {newHash, cost + newCost};
        tal_add(moves, newHS);

        return moves;
}

talhashstate getMoves(hashstate hs) {
        talhashstate moves = tal_init();
        state s = hashtostate(hs.state);
        u64 cost = hs.cost;

        // check hallway moves
        for (int i = 0; i < HALLWAY_LENGTH; i++) {
                if (s.hallway[i] == 0) continue;

                talhashstate hallwayMoves = moveFromHall(s, cost, i);
                for (int j = 0; j < (int)hallwayMoves.length; j++) {
                        tal_add(moves, hallwayMoves.array[j]);
                }
                tal_destroy(hallwayMoves);
        }

        // check room moves
        for (int i = 0; i < ROOM_SIZE; i++) {
                for (int j = 0; j < NUM_ROOMS; j++) {
                        if (s.rooms[j][i] == 0) continue;

                        talhashstate roomMoves = moveFromRoom(s, cost, j, i);
                        for (int j = 0; j < (int)roomMoves.length; j++) {
                                tal_add(moves, roomMoves.array[j]);
                        }
                        tal_destroy(roomMoves);
                }
        }

        return moves;
}

u64 dijkstra(state init) {
        // Get end state
        u64 endHash = getEndState();

        talhashstate visited = tal_init();
        talhashstate queue = tal_init();

        u64 initHash = statetohash(init);
        hashstate initHS = {initHash, 0};
        tal_add(visited, initHS);
        tal_add(queue, initHS);

        while (queue.length > 0) {
                hashstate curS = tal_pop_front(queue);

                if (curS.state == endHash) {
                        return curS.cost;
                }

                talhashstate moves = getMoves(curS);
                for (int i=0; i<(int)moves.length; i++) {
                        if (insertVisited(&visited, moves.array[i])) {
                                insertQueue(&queue, moves.array[i]);
                        }
                }
        }

        printf("ERROR: dijkstra failed to find a path\n");
        return 0;
}

void unfoldRooms(state *s) {
        // Move apods to bottom of rooms
        for (int i=0; i<ROOM_SIZE; i++) {
                s->rooms[i][3] = s->rooms[i][1];
        }

        // add to tops of rooms: D C B A  or  4 3 2 1
        //                       D B A C      4 2 1 3
        u8 newApods[4][2] = {{4, 4}, {3, 2}, {2, 1}, {1, 3}};
        for (int i=0; i<ROOM_SIZE; i++) {
                s->rooms[i][1] = newApods[i][0];
                s->rooms[i][2] = newApods[i][1];
        }
}

void part1(struct input input) {
        state initState = input.initState;

        // Add filler to bottom of room to simulate room size 2
        for (int i=0; i<ROOM_SIZE; i++) {
                initState.rooms[i][2] = i + 1;
                initState.rooms[i][3] = i + 1;
        }
        // printState(initState);

        u64 cost = dijkstra(initState);

        printf("Part 1: %lu\n\n", cost);
}

void part2(struct input input) {
        state initState = input.initState;
        unfoldRooms(&initState);
        // printState(initState);

        u64 cost = dijkstra(initState);

        printf("Part 2: %lu\n\n", cost);
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

