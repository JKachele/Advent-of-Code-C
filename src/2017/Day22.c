/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Oct 22, 2026 08:36:32 EDT
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
#include "../util/hashtable.h"

#define INPUT_BUFFER_SIZE 1024

enum direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
};

enum health {
        CLEAN,
        WEAK,
        INFECT,
        FLAG
};

struct state {
        ivec2 pos;
        enum direction dir;
};

struct input {
        ivec2 size;
        bool **grid;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printTable(hashtable table) {
        for (u64 i = 0; i < table.capacity; i++) {
                ht_entry *cur = table.table[i];
                int n = 0;
                while (cur != NULL) {
                        ivec2 key = *(ivec2*)cur->key;
                        bool item = *(bool*)cur->item;
                        if (n > 0) { printf("\t"); }
                        printf("%lu: (%d, %d) - %s\n", i, key.x, key.y, item ? "true" : "false");
                        cur = cur->next;
                        n++;
                }
        }
        printf("Calacity = %lu, Size = %lu, Threshold = %lu\n",
                        table.capacity, table.size, table.threshold);
}

void printTable2(hashtable table) {
        char *healths[] = {"CLEAN", "WEAK", "INFECT", "FLAG"};
        for (u64 i = 0; i < table.capacity; i++) {
                ht_entry *cur = table.table[i];
                while (cur != NULL) {
                        ivec2 key = *(ivec2*)cur->key;
                        enum health item = *(enum health*)cur->item;
                        printf("%lu: (%d, %d) - %s\n", i, key.x, key.y, healths[item]);
                        cur = cur->next;
                }
        }
        printf("Calacity = %lu, Size = %lu\n", table.capacity, table.size);
}

u64 posTohash(const void *key, u64 mod) {
        ivec2 pos = *(ivec2*)key;
        u64 hash = 0;
        hash += (u64)pos.x;
        hash <<= 32;
        hash += (u64)pos.y;
        return hash % mod;
}

int key_cmp(const void *key1, const void *key2) {
        ivec2 pos1 = *(ivec2*)key1;
        ivec2 pos2 = *(ivec2*)key2;

        if (ivec2Eq(pos1, pos2)) {
                return 0;
        }
        return 1;
}

bool move(hashtable *table, struct state *state) {
        const ivec2 Dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};

        // Try to remove entry for current position
        // If no entry exists, node is clean, else node is dirty
        bool infected;
        if (ht_remove(table, &state->pos) == HT_SUCCESS) {
                // Node infected. Turn right
                state->dir = (state->dir + 1) % 4;
                infected = false;
        } else {
                // Node clean. Add node to table to mark as infected and turn left.
                ivec2 *key = malloc(sizeof(ivec2));
                bool *item = malloc(1);
                *key = state->pos;
                *item = true;
                ht_add(table, sizeof(ivec2), key, item);
                state->dir = (state->dir + 3) % 4;
                infected = true;
        }

        // Move forward 1
        state->pos = ivec2Add(state->pos, Dirs[state->dir]);

        return infected;
}

bool move2(hashtable *table, struct state *state) {
        const ivec2 Dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};

        // Get entry for current pos.
        // If no entry exists, node is clean
        bool infected = false;
        ht_entry *entry = ht_find(table, &state->pos);
        if (entry == NULL) {
                // Node clean. Add node to table to mark as weak and turn left
                ivec2 *key = malloc(sizeof(ivec2));
                *key = state->pos;
                enum health *item = malloc(sizeof(enum health));
                *item = WEAK;
                ht_add(table, sizeof(ivec2), key, item);
                state->dir = (state->dir + 3) % 4;
        } else {
                enum health health = *(enum health*)entry->item;
                if (health == WEAK) {
                        // Weakened nodes become infected. No turn
                        *(enum health*)entry->item = INFECT;
                        infected = true;
                } else if (health == INFECT) {
                        // Infected nodes become flagged. Turn Right
                        *(enum health*)entry->item = FLAG;
                        state->dir = (state->dir + 1) % 4;
                } else /* health == FLAG */ {
                        // Flagged nodes become clean. Remove entry from table and turn 180
                        ht_remove(table, &state->pos);
                        state->dir = (state->dir + 2) % 4;
                }
        }

        // Move forward 1
        state->pos = ivec2Add(state->pos, Dirs[state->dir]);

        return infected;
}

void part1(struct input *input) {
        const int NumSteps = 10000;
        ivec2 size = input->size;
        bool (*grid)[size.x] = (bool(*)[size.x])input->grid;

        // Create hashtable and add current on nodes
        hashtable table = ht_create(1000, posTohash, key_cmp);
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x]) {
                                ivec2 *key = malloc(sizeof(ivec2));
                                key->x = x;
                                key->y = y;
                                bool *item = malloc(1);
                                *item = true;
                                ht_add(&table, sizeof(ivec2), key, item);
                        }
                }
        }

        struct state state = {{{size.x / 2, size.y / 2}}, UP};

        int numInfected = 0;
        for (int i = 0; i < NumSteps; i++) {
                if (move(&table, &state)) {
                        numInfected++;
                }
        }
        // printTable(table);

        printf("Part 1: Bursts Infected = %d\n\n", numInfected);
}

void part2(struct input *input) {
        const int NumSteps = 10000000;
        ivec2 size = input->size;
        bool (*grid)[size.x] = (bool(*)[size.x])input->grid;

        // Create hashtable and add current on nodes
        hashtable table = ht_create(1000, posTohash, key_cmp);
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x]) {
                                ivec2 *key = malloc(sizeof(ivec2));
                                key->x = x;
                                key->y = y;
                                enum health *item = malloc(sizeof(enum health));
                                *item = INFECT;
                                ht_add(&table, sizeof(ivec2), key, item);
                        }
                }
        }

        struct state state = {{{size.x / 2, size.y / 2}}, UP};

        int numInfected = 0;
        for (int i = 0; i < NumSteps; i++) {
                if (move2(&table, &state)) {
                        numInfected++;
                }
        }
        // printTable2(table);

        printf("Part 2: Bursts Infected = %d\n", numInfected);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.grid = malloc(size.x * size.y);
        bool (*grid)[size.x] = (bool(*)[size.x])input.grid;

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < (int)strlen(str); x++) {
                        grid[y][x] = (str[x] == '#');
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day22.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day22.txt";
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

