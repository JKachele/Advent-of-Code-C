/*************************************************
 *File----------Day18.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 09, 2026 14:47:03 UTC
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
#include "../lib/tllist.h"

#define INPUT_BUFFER_SIZE 16384

typedef struct move {
        ivec2 pos[4];
        int64 dist;
        bool keys[26];
        u32 keysId; // Bit representation of keys
        int numKeys;
} move;
typedef tll(move) tllmove;

struct input {
        ivec2 size;
        char **map;
        ivec2 start;
        int numKeys;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, char map[][size.x]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        printf("%c", map[y][x]);
                }
                printf("\n");
        }
}

void printMove(move m) {
        printf("Move: ");
        for (int i = 1; i < 4; i++)
                printf("(%d, %d) ", m.pos[i].x, m.pos[i].y);
        printf("Distance: %ld\n\tKeys: %d [ ",m.dist, m.numKeys);
        for (int i = 0; i < 26; i++)
                if (m.keys[i]) printf("%c ", 'a' + i);
        printf("]\n");
}

u32 keysToId(bool keys[26]) {
        u32 id = 0;
        for (int i = 0; i < 26; i++) {
                id <<= 1;
                id += (keys[i] ? 1 : 0);
        }
        return id;
}

bool moveEq(move a, move b) {
        if (a.numKeys != b.numKeys || a.keysId != b.keysId)
                return false;
        bool equal = true;
        for (int i = 0; i < 4; i++) {
                if (!ivec2Eq(a.pos[i], b.pos[i])) {
                        equal = false;
                        break;
                }
        }
        return equal;
}

void setMap(ivec2 size, char map[][size.x], bool keys[26]) {
        // Remove all keys and doors we have from map
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        char c = map[y][x];
                        if (c == '#' || c == '.')
                                continue;
                        if ((c >= 'a' && keys[c-'a']) || (c < 'a' && keys[c-'A']))
                                map[y][x] = '.';
                }
        }
}

tllmove getMoves(ivec2 size, char map[][size.x], move start, int vault) {
        const ivec2 Dirs[] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
        tllmove moves = tll_init();
        
        // Create a copy of the map to update with collected keys
        char (*mapCopy)[size.x] = (char(*)[size.x])malloc(size.x * size.y * sizeof(char));
        memcpy(mapCopy, map, size.x * size.y * sizeof(char));
        setMap(size, mapCopy, start.keys);

        // Do a bfs search for all keys accessable from the current position
        tllmove queue = tll_init();
        tll_push_back(queue, start);

        while (queue.length > 0) {
                move m = tll_pop_front(queue);
                mapCopy[m.pos[vault].y][m.pos[vault].x] = '#';

                for (int i = 0; i < 4; i++) {
                        ivec2 newPos = ivec2Add(m.pos[vault], Dirs[i]);
                        char c = mapCopy[newPos.y][newPos.x];
                        if (c == '#' || (c >= 'A' && c <= 'Z'))
                                continue;

                        if (c == '.') {
                                move new = m;
                                new.pos[vault] = newPos;
                                new.dist++;
                                tll_push_back(queue, new);
                        } else if (c >= 'a' && c <= 'z') {
                                move new = m;
                                new.pos[vault] = newPos;
                                new.dist++;
                                new.numKeys++;
                                for (int j = 0; j < 26; j++)
                                        new.keys[j] = start.keys[j];
                                new.keys[c - 'a'] = true;
                                new.keysId = keysToId(new.keys);
                                tll_push_back(moves, new);
                        }
                }
        }

        return moves;
}

void addMove(tllmove *moves, tllmove visited, move m) {
        // Add moves only if they havent been seen before
        tll_foreach(visited, it) {
                if (moveEq(it->item, m))
                        return;
        }

        // If move is already in the queue, If the current move is shorter, replace the existing one
        tll_foreach(*moves, it) {
                if (moveEq(it->item, m)) {
                        if (it->item.dist > m.dist) {
                                tll_remove(*moves, it);
                                break;
                        }
                        return;
                }
        }

        // Order queue from shortest distance to longest distance
        tll_foreach(*moves, it) {
                if (it->item.dist == m.dist && it->item.numKeys > m.numKeys) {
                        tll_insert_before(*moves, it, m);
                        return;
                }
                if (it->item.dist > m.dist) {
                        tll_insert_before(*moves, it, m);
                        return;
                }
        }
        tll_push_back(*moves, m);
}

int64 bfsKeys(ivec2 size, char map[][size.x], ivec2 start[4], int numKeys, bool part1) {
        tllmove queue = tll_init();
        move init = {0};
        for (int i = 0; i < 4; i++)
                init.pos[i] = start[i];
        tll_push_back(queue, init);

        tllmove visited = tll_init();

        while (queue.length > 0) {
                move cur = tll_pop_front(queue);
                // printMove(cur);
                tll_push_back(visited, cur);

                // First move with all keys collected is guaranteed to be the shortest path
                if (cur.numKeys == numKeys)
                        return cur.dist;

                for (int i = 0; i < (part1 ? 1 : 4); i++) {
                        tllmove newMoves = getMoves(size, map, cur, i);
                        tll_foreach(newMoves, it) {
                                addMove(&queue, visited, it->item);
                                tll_remove(newMoves, it);
                        }
                }
        }

        // Should never reach here. Only reach here if no path was found
        printf("ERROR: No path found with all keys collected\n");
        return -1;
}

void setNewVaults(ivec2 size, char map[][size.x], ivec2 vaults[4], ivec2 start) {
        const ivec2 Dirs[] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
        const ivec2 Corners[] = {{{1, -1}}, {{1, 1}}, {{-1, 1}}, {{-1, -1}}};

        map[start.y][start.x] = '#';
        for (int i = 0; i < 4; i++) {
                vaults[i] = ivec2Add(start, Corners[i]);
                ivec2 v = ivec2Add(start, Dirs[i]);
                map[v.y][v.x] = '#';
        }
}

void part1(struct input *input) {
        ivec2 size = input->size;
        char (*map)[size.x] = (char(*)[size.x])malloc(size.x * size.y * sizeof(char));
        memcpy(map, input->map, size.x * size.y * sizeof(char));

        ivec2 start[4] = {input->start, {{0}}, {{0}}, {{0}}};
        int64 dist = bfsKeys(size, map, start, input->numKeys, true);

        printf("Part 1: %ld\n\n", dist);
}

void part2(struct input *input) {
        ivec2 size = input->size;
        char (*map)[size.x] = (char(*)[size.x])malloc(size.x * size.y * sizeof(char));
        memcpy(map, input->map, size.x * size.y * sizeof(char));

        ivec2 start[4];
        setNewVaults(size, map, start, input->start);
        int64 dist = bfsKeys(size, map, start, input->numKeys, false);

        printf("Part 2: %ld\n", dist);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll), ll->length}};
        input.size = size;
        input.map = malloc(size.x * size.y * sizeof(char));
        char (*map)[size.x] = (char(*)[size.x])input.map;

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < size.x; x++) {
                        char c = str[x];
                        map[y][x] = c;
                        if (c == '@') {
                                input.start = (ivec2){{x, y}};
                                map[y][x] = '.';
                        } else if (c >= 'a' && c <= 'z') {
                                input.numKeys++;
                        }
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day18.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day18.txt";
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

