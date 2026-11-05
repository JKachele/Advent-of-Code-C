/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Oct 28, 2026 11:02:51 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <math.h>
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
#include "../util/md5.h"
#include "../util/quicksort.h"

#define INPUT_BUFFER_SIZE 1024
#define NUM_FLOORS 4
#define MAX_ITEMS 8     // Max number of item types

typedef tal(u64) talu64;
typedef tal(u32) talu32;
typedef tal(ivec2) talivec2;

struct state {
        int floor;
        bool floorGens[NUM_FLOORS][MAX_ITEMS];
        bool floorChips[NUM_FLOORS][MAX_ITEMS];
        int numGens[NUM_FLOORS];
        int numChips[NUM_FLOORS];
        int moves;
        int score;
        u8 numMoved;
        struct state *next;
};
typedef tal(struct state) talstate;

typedef struct {
        u64 enc[2];
} enc;
typedef tal(enc) talenc;

struct input {
        struct state initState;
        int numTypes;
};

void printState(struct state state) {
        printf("Moves: %d\n", state.moves);
        printf("Elevator on floor %d\n", state.floor);
        for (int i = 0; i < NUM_FLOORS; i++) {
                printf("Floor %d: (%d, %d)\n", i, state.numChips[i], state.numGens[i]);
                for (int j = 0; j < MAX_ITEMS; j++) {
                        if (state.floorChips[i][j]) {
                                printf("\tChip %d\n", j);
                        }
                        if (state.floorGens[i][j]) {
                                printf("\tGenerator %d\n", j);
                        }
                }
        }
        printf("\n");
}

int wordCount(char *str) {
        int numWords = 1;
        bool space = false;
        for (int i = 0; i < (int)strlen(str); i++) {
                if (str[i] == ' ' && !space) {
                        space = true;
                        numWords++;
                } else if (str[i] != ' ') {
                        space = false;
                }
        }
        return numWords;
}

void splitLine(char *line, char *words[], int numWords) {
        char *tok = strtok(line, " ");
        for (int i = 0; i < numWords; i++) {
                words[i] = malloc(strlen(tok) + 1);
                strcpy(words[i], tok);
                tok = strtok(NULL, " ");
        }

}

void freeWords(char *words[], int numWords) {
        for (int i = 0; i < numWords; i++) {
                free(words[i]);
        }
}

// 63-60 59-45 44-30 29-15 14-0
// Elev   F0    F1    F2    F3
// Floors: [14-7]: Chips, [6-0]: Generators
// u64 encodeState(struct state state) {
//         const u64 FloorMask = 0xFFF;
//         u64 code = state.floor;
//
//         for (int i = 0; i < NUM_FLOORS; i++) {
//                 code <<= 15;
//                 u64 floor = 0;
//                 for (int j = 0; j < MAX_ITEMS; j++) {
//                         if (state.floorChips[i][j]) {
//                                 floor |= (1 << (j + 7));
//                         }
//                         if (state.floorGens[i][j]) {
//                                 floor |= (1 << j);
//                         }
//                 }
//                 code |= (floor & FloorMask);
//         }
//         return code;
// }

int pairCmp(void *a, void *b) {
        ivec2 vecA = *(ivec2*)a;
        ivec2 vecB = *(ivec2*)b;
        if (vecA.b > vecB.b) {
                return 1;
        } else if (vecA.b == vecB.b) {
                return 0;
        } else {
                return -1;
        }
}

int encEq(enc a, enc b) {
        if (a.enc[0] == b.enc[0]) {
                if (a.enc[1] == b.enc[1]) {
                        return 0;
                } else if (a.enc[1] > b.enc[1]) {
                        return 1;
                } else {
                        return -1;
                }
        } else if (a.enc[0] > b.enc[0]) {
                return 1;
        } else {
                return -1;
        }
}

// Encode state so that all pairs are interchangeable
enc encodeState(struct state state) {
        enc code = {0};
        code.enc[0] |= state.floor;

        // Get chip-generator pairs
        ivec2 pairs[MAX_ITEMS] = {0};
        for (int i = 0; i < NUM_FLOORS; i++) {
                for (int j = 0; j < MAX_ITEMS; j++) {
                        if (state.floorChips[i][j]) {
                                pairs[j].a = i + 1;
                        }
                        if (state.floorGens[i][j]) {
                                pairs[j].b = i + 1;
                        }
                }
        }

        // Sort pairs by generator floor
        // void quicksort(void *arr, size_t size, int first, int last, int (*cmp)(void*, void*));
        quicksort(pairs, sizeof(ivec2), 0, MAX_ITEMS-1, pairCmp);

        /* Each floor is 24 bits: 8 slots of 3 bits each. If a floor contains a chip,
         * a slot will be used to show that chip's generator floor + 1 (1-4). All 0s indicate that
         * a slot is empty. The slots are ordered from smallest floor to largest floor.
         */
        for (int i = 0; i < NUM_FLOORS; i++) {
                u64 floorEnc = 0;
                for (int j = 0; j < MAX_ITEMS; j++) {
                        if (pairs[j].a == 0) { continue; }
                        if (pairs[j].a == i + 1) {
                                floorEnc <<= 3;
                                floorEnc |= pairs[j].b & 0x7;
                        }
                }
                code.enc[i >> 1] <<= 24;
                code.enc[i >> 1] |= floorEnc;
        }
        return code;
}

int stateScore(struct state state) {
        int score = 0;
        for (int i = 0; i < NUM_FLOORS; i++) {
                score += (i + 1) * (state.numChips[i] + state.numGens[i]);
        }
        return score;
}

bool checkVisited(struct state state, talenc *visited) {
        enc code = encodeState(state);
        int left = 0;
        int right = visited->length - 1;
        while (left <= right) {
                int mid = left + ((right - left) / 2);
                enc cur = visited->array[mid];
                if (encEq(cur, code) == 0) {
                        return true;
                } else if (encEq(cur, code) == -1) {
                        left = mid + 1;
                } else {
                        right = mid - 1;
                }
        }
        tal_insert(*visited, left, code);
        return false;
}

void addStateToQueue(struct state **queue, talenc *visited, struct state state) {
        // If already visited this state, don't add to queue
        if (checkVisited(state, visited)) {
                return;
        }

        struct state *toAdd = malloc(sizeof(struct state));
        *toAdd = state;
        toAdd->next = NULL;

        // If queue empty, add to head
        // Prioritize move count and the how close to the end state it is
        // The lower the moves and the higher the score is, the closer to the front it is
        if ((*queue == NULL) || (toAdd->moves == (*queue)->moves && toAdd->score > (*queue)->score) ||
                        (toAdd->moves < (*queue)->moves)) {
                toAdd->next = *queue;
                *queue = toAdd;
                return;
        }

        struct state *prev = *queue;
        struct state *cur = prev->next;
        while (cur != NULL) {
                if ((toAdd->moves == cur->moves && toAdd->score > cur->score) ||
                                (toAdd->moves < cur->moves)) {
                        toAdd->next = cur;
                        prev->next = toAdd;
                        return;
                }
                prev = cur;
                cur = cur->next;
        }
        prev->next = toAdd;
}

bool floorEmpty(struct state state, int floor) {
        return (state.numChips[floor] == 0) && (state.numGens[floor] == 0);
}

bool isEndState(struct state state) {
        for (int i = 0; i < NUM_FLOORS - 1; i++) {
                if (!floorEmpty(state, i)) {
                        return false;
                }
        }
        return true;
}

bool isValidState(struct state state, int prevFloor, int nextFloor) {
        for (int i = 0; i < 2; i++) {
                int floor = (i == 0) ? prevFloor : nextFloor;
                if (state.numChips[floor] == 0 || state.numGens[floor] == 0) {
                        // If floor is empty or only has either chips or gens, it is valid
                        continue;
                }
                if (state.numChips[floor] > state.numGens[floor]) {
                        // If floor has both, and there are more chips, cannot be valid
                        return false;
                }

                // Check if each chip has its generator
                for (int j = 0; j < MAX_ITEMS; j++) {
                        if (state.floorChips[floor][j] && !state.floorGens[floor][j]) {
                                return false;
                        }
                }
        }
        return true;
}

void moveItem(struct state *cur, ivec2 item, int prevFloor, int floor) {
        if (item.b == 0) {
                cur->floorChips[prevFloor][item.a] = false;
                cur->numChips[prevFloor]--;
                cur->floorChips[floor][item.a] = true;
                cur->numChips[floor]++;
        } else {
                cur->floorGens[prevFloor][item.a] = false;
                cur->numGens[prevFloor]--;
                cur->floorGens[floor][item.a] = true;
                cur->numGens[floor]++;
        }
}

void getNextStates(struct state **queue, talenc *visited, struct state cur) {
        // List of items on floor, {index, 0: chip 1: Gen}
        talivec2 items = tal_init();
        for (int i = 0; i < MAX_ITEMS; i++) {
                if (cur.floorChips[cur.floor][i]) {
                        ivec2 item = {{i, 0}};
                        tal_add(items, item);
                }
                if (cur.floorGens[cur.floor][i]) {
                        ivec2 item = {{i, 1}};
                        tal_add(items, item);
                }
        }

        // Test moving every combo of 1 or 2 items up or down 1 floor
        talstate nextStates = tal_init();
        bool moved2Up = false;
        bool moved1Down = false;
        for (u64 i = 0; i < items.length; i++) {
                for (u64 j = i; j < items.length; j++) {
                        for (int floor = cur.floor - 1; floor <= cur.floor + 1; floor += 2) {
                                if ((floor >= NUM_FLOORS) || (floor < 0)) { continue; }

                                // Optimization: If 1st or 2nd floors are empty, No need to
                                // move items down there again
                                if (floorEmpty(cur, 0)) {
                                        if (floor == 0) { continue; }
                                        if (floorEmpty(cur, 1) && floor == 1) { continue; }
                                }

                                struct state next = cur;
                                moveItem(&next, items.array[i], cur.floor, floor);
                                next.numMoved = 1;
                                if (i != j) {
                                        moveItem(&next, items.array[j], cur.floor, floor);
                                        next.numMoved = 2;
                                }
                                next.floor = floor;
                                next.score = stateScore(next);
                                next.moves++;

                                if (isValidState(next, cur.floor, next.floor)) {
                                        if ((i != j) && (next.floor > cur.floor)) {
                                                moved2Up = true;
                                        }
                                        if ((i == j) && (next.floor < cur.floor)) {
                                                moved1Down = true;
                                        }
                                        tal_add(nextStates, next);
                                }
                        }

                }
        }

        // Optimization: Moving 2 items up is always better than moving 1 up and
        // moving 1 item down is always better than moving 2 items down.
        tal_for(nextStates, i) {
                struct state next = nextStates.array[i];
                if (moved2Up && next.floor > cur.floor && next.numMoved == 1) { continue; }
                if (moved1Down && next.floor < cur.floor && next.numMoved == 2) { continue; }
                addStateToQueue(queue, visited, next);
        }
        tal_destroy(nextStates);
}

int fastestToTop(struct state initState) {
        talenc visited = tal_init();
        tal_add(visited, encodeState(initState));

        struct state *queue = malloc(sizeof(struct state));
        *queue = initState;

        u64 numItterations = 0;
        int numMoves = -1;
        while (queue != NULL) {
                struct state *cur = queue;
                queue = cur->next;
                // printState(cur);
                
                if (isEndState(*cur)) {
                        numMoves = cur->moves;
                        free(cur);
                        break;
                }
                getNextStates(&queue, &visited, *cur);
                free(cur);
                numItterations++;
        }

        // printf("Num Itterations = %lu\n", numItterations);
        // printf("Num Unique State = %lu\n", visited.length);
        tal_destroy(visited);
        while (queue != NULL) {
                struct state *cur = queue;
                queue = cur->next;
                free(cur);
        }
        return numMoves;
}

void part1(struct input *input) {
        struct state initState = input->initState;
        // printState(initState);

        int numMoves = fastestToTop(initState);

        printf("Part 1: Moves = %d\n\n", numMoves);
}

void part2(struct input *input) {
        struct state initState = input->initState;
        // return;

        // Add 2 new chips and generators to 1st floor
        for (int i = 0; i < 2; i++) {
                initState.floorChips[0][input->numTypes + i] = true;
                initState.floorGens[0][input->numTypes + i] = true;
                initState.numChips[0]++;
                initState.numGens[0]++;
        }
        // printState(initState);

        int numMoves = fastestToTop(initState);

        printf("Part 2: Moves = %d\n", numMoves);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        talu32 hashToIndex = tal_init();

        struct state state = {0};

        llNode *cur = ll->head;
        u8 itemIndex = 0;
        for (int i = 0; i < NUM_FLOORS; i++) {
                char *str = (char*)cur->data;
                int numWords = wordCount(str);
                char *words[numWords];
                splitLine(str, words, numWords);

                int numItems = (numWords < 8) ? ((numWords == 6) ? 0 : 1) : (numWords - 5) / 3;
                int word = 5;
                for (int j = 0; j < numItems; j++) {
                        bool isChip = words[word+1][0] == 'm';
                        char *name = words[word];
                        if (isChip) {
                                name = strtok(words[word], "-");
                        }
                        u32 hash = md5_32(name);
                        // printf("%s : %x\n", name, item.hash);

                        int index = -1;
                        tal_for(hashToIndex, i) {
                                if (hashToIndex.array[i] == hash) {
                                        index = i;
                                }
                        }
                        if (index == -1) {
                                tal_add(hashToIndex, hash);
                                index = hashToIndex.length - 1;
                        }

                        if (isChip) {
                                state.floorChips[i][index] = true;
                                state.numChips[i]++;
                        } else {
                                state.floorGens[i][index] = true;
                                state.numGens[i]++;
                        }

                        word += 3;
                        if (j == numItems - 2) { word++; } // Account for "and" in the last item
                }

                freeWords(words, numWords);
                cur = cur->next;
        }
        state.score = stateScore(state);

        input.initState = state;
        input.numTypes = hashToIndex.length;
        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2016/Day11.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2016/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        } else {
                char *file = "assets/inputs/2016/Day11.txt";
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

