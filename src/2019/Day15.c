/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Apr 07, 2026 17:02:10 UTC
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
#include "../util/talist.h"
#include "../util/Intcode2019.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 16384

typedef enum state {
        WALL,
        OPEN,
        OXYGEN,
        START,
        VISITED
} state;

typedef struct cell {
        ivec2 pos;
        state state;
} cell;
typedef tal(cell) talcell;

struct input {
        talint64 intcode;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, int map[][size.x]) {
        if (!Debug) return;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (map[y][x] == WALL)
                                printf("\xE2\x96\x92");
                        else if (map[y][x] == OPEN)
                                printf(" ");
                        else if (map[y][x] == OXYGEN)
                                printf("X");
                        else if (map[y][x] == START)
                                printf("S");
                }
                printf("\n");
        }
}

bool addCell(talcell *map, ivec2 pos, int state) {
        for (int i = 0; i < (int)map->length; i++) {
                if (ivec2Eq(map->array[i].pos, pos))
                        return true;
        }
        cell c = {pos, state};
        tal_add(*map, c);
        return false;
}

bool opposite(int dir1, int dir2) {
        switch (dir1) {
        case 0:
                return dir2 == 1;
        case 1:
                return dir2 == 0;
        case 2:
                return dir2 == 3;
        case 3:
                return dir2 == 2;
        default:
                return false;
        }
}

ivec2 generateMap(talint64 *intcode, haltmode state, talcell *cells, ivec2 pos, int dir, int depth) {
        const ivec2 Offsets[4] = {{{0, -1}}, {{0, 1}}, {{-1, 0}}, {{1, 0}}};
        static int maxDepth = 0;
        if (depth > maxDepth)
                maxDepth = depth;

        ivec2 oxPos = {0};

        // Loop until hit a fork or hit a dead end
        for (;;) {
                // printf("(%d, %d): %d\n", pos.x, pos.y, depth);
                // Test each direction
                int numPaths = 0;
                bool dirs[5] = {0}; // dirs[4] is found oxygen
                for (int i = 0; i < 4; i++) {
                        if (opposite(dir, i))
                                continue;
                        talint64 intCopy = copyIntcode(*intcode);
                        talint64 inputs = tal_init();
                        talint64 outputs = tal_init();

                        tal_add(inputs, i + 1); // +1 because inputs are 1-4 not 0-3
                        runIntcode(&intCopy, state, &inputs, &outputs);
                        int out = outputs.array[0];

                        tal_destroy(intCopy);
                        tal_destroy(inputs);
                        tal_destroy(outputs);

                        ivec2 newPos = ivec2Add(pos, Offsets[i]);
                        // If cell already visited, dont visit it again
                        if (addCell(cells, newPos, out))
                                continue;

                        if (out == 1) {
                                dirs[i] = true;
                                numPaths++;
                        } else if (out == 2) {
                                dirs[i] = true;
                                dirs[4] = true;
                                oxPos = newPos;
                                numPaths++;
                        }
                }
                if (numPaths == 0)
                        break;
                if (numPaths == 1) {
                        for (int i = 0; i < 4; i++)
                                if (dirs[i])
                                        dir = i;
                        pos = ivec2Add(pos, Offsets[dir]);
                        talint64 inputs = tal_init();
                        talint64 outputs = tal_init();
                        tal_add(inputs, dir + 1);
                        state = runIntcode(intcode, state, &inputs, &outputs);
                        continue;
                }
                // If multiple paths, recurse with the new directions
                // Only recurse extra paths, Leave one to continue. This reduces recursive calls.
                int i = 0;
                for (i = 0; i < 4; i++) {
                        if (!dirs[i])
                                continue;
                        if (numPaths == 1)
                                break;

                        numPaths--;
                        talint64 intCopy = copyIntcode(*intcode);
                        talint64 inputs = tal_init();
                        talint64 outputs = tal_init();
                        tal_add(inputs, i + 1);
                        haltmode newState = runIntcode(&intCopy, state, &inputs, &outputs);
                        ivec2 newPos = ivec2Add(pos, Offsets[i]);
                        ivec2 newOxPos = generateMap(&intCopy, newState, cells, newPos, i, depth + 1);
                        if (newOxPos.x != 0 || newOxPos.y != 0)
                                oxPos = newOxPos;
                        tal_destroy(intCopy);
                        tal_destroy(inputs);
                        tal_destroy(outputs);
                }
                dir = i;
                pos = ivec2Add(pos, Offsets[dir]);
                talint64 inputs = tal_init();
                talint64 outputs = tal_init();
                tal_add(inputs, dir + 1);
                state = runIntcode(intcode, state, &inputs, &outputs);
        }
        if (depth == 0)
                debugp("Max depth: %d\n", maxDepth);

        return oxPos;
}

int bfs(ivec2 size, int map[][size.x], ivec2 start) {
        const ivec2 Offsets[4] = {{{0, -1}}, {{0, 1}}, {{-1, 0}}, {{1, 0}}};

        // Using cell to track position. State is distance from start
        talcell queue = tal_init();

        // Add start cell to queue
        cell init = {start, 0};
        tal_add(queue, init);

        int curDist = 0;
        while (queue.length > 0) {
                cell cur = tal_pop_front(queue);
                curDist = cur.state;

                // Set state of current cell to visited. Prevents backtracking
                map[cur.pos.y][cur.pos.x] = VISITED;

                // Check 4 directions
                for (int i = 0; i < 4; i++) {
                        ivec2 newPos = ivec2Add(cur.pos, Offsets[i]);

                        // If new cell is the oxygen, return distance
                        if (map[newPos.y][newPos.x] == OXYGEN)
                                return cur.state + 1;
                        
                        // if new cell is open and not visited, add it to queue
                        if (map[newPos.y][newPos.x] == OPEN) {
                                cell newCell = {newPos, cur.state + 1};
                                tal_add(queue, newCell);
                        }
                }
        }

        return curDist;
}

void part1_2(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);

        talcell cells = tal_init();
        cell c = {(ivec2){{0, 0}}, 3};
        tal_add(cells, c);
        ivec2 oxPos = generateMap(&intcode, (haltmode){0}, &cells, (ivec2){0}, -1, 0);
        debugp("Found oxygen at (%d, %d)\n", oxPos.x, oxPos.y);

        // Get map size
        ivec2 min = {0};
        ivec2 max = {0};
        for (int i = 0; i < (int)cells.length; i++) {
                cell c = cells.array[i];
                if (c.pos.x < min.x) min.x = c.pos.x;
                if (c.pos.y < min.y) min.y = c.pos.y;
                if (c.pos.x > max.x) max.x = c.pos.x;
                if (c.pos.y > max.y) max.y = c.pos.y;
        }
        ivec2 size = {{max.x - min.x + 1, max.y - min.y + 1}};

        // Generate map and set cells
        int (*map)[size.x] = (int(*)[size.x])calloc(size.x * size.y, sizeof(int));
        ivec2 start = {0};
        oxPos = ivec2Sub(oxPos, min);
        for (int i = 0; i < (int)cells.length; i++) {
                cell c = cells.array[i];
                ivec2 pos = ivec2Sub(c.pos, min);
                map[pos.y][pos.x] = c.state;
                if (c.state == 3)
                        start = pos;
        }
        printMap(size, map);

        int distance = bfs(size, map, start);

        printf("Part 1: %d\n\n", distance);
        /************************ Part 2 ************************/
        // Reset map. Set all visited cells to open
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (map[y][x] == VISITED)
                                map[y][x] = OPEN;
                }
        }

        // bfs starting at oxygen cell
        // With no goal, it will output the distance to the farthest cell
        distance = bfs(size, map, oxPos);

        printf("Part 2: %d\n", distance);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        char *str = (char*)ll->head->data;

        char *code = strtok(str, ",");
        while (code != NULL) {
                int64 num = strtoll(code, NULL, 10);
                tal_add(input.intcode, num);
                code = strtok(NULL, ",");
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
        llist_free(ll);
        clock_t parse = clock();
        part1_2(&input);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

