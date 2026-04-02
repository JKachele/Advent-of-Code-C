/*************************************************
 *File----------Day11.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 02, 2026 15:30:27 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/talist.h"
#include "../util/Intcode2019.h"
#include "../util/vector.h"
// #include "../lib/tllist.h"

#define INPUT_BUFFER_SIZE 4096

typedef struct panel {
        u64 posId; // [63:32] = y + 0xFFFF, [31:0] = x + 0xFFFF;
        bool color; // False = black, True = white
} panel;
typedef tal(panel) talpanel;

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

talint64 copyList(talint64 list) {
        talint64 newList = tal_init();
        for (int i = 0; i < (int)list.length; i++) {
                tal_add(newList, list.array[i]);
        }
        return newList;
}

u64 posToId(ivec2 pos) {
        u64 id = pos.y + 0xFFFF;
        id <<= 32;
        id += pos.x + 0xFFFF;
        return id;
}

ivec2 idToPos(u64 id) {
        ivec2 p = {0};
        u64 mask = 0xFFFFFFFF;
        p.x = (id & mask) - 0xFFFF;
        id >>= 32;
        p.y = (id & mask) - 0xFFFF;
        return p;
}

bool findPanel(talpanel *panels, u64 pos, int color) { // Color: -1 = do not color
        // Binary search for the panel with the given position. If not found, add a new one.
        int low = 0;
        int high = panels->length - 1;

        while (low <= high) {
                int index = low + ((high - low) / 2);
                panel *p = &panels->array[index];

                if (p->posId == pos) {
                        if (color != -1)
                                p->color = (color == 1);
                        return p->color;
                }

                if (p->posId > pos)
                        high = index - 1;
                else
                        low = index + 1;
        }

        if (color != -1) {
                panel newPanel = {pos, color};
                tal_insert(*panels, low, newPanel);
                return (color == 1);
        }
        return false;
}

ivec2 move(ivec2 pos, int dir) {
        switch (dir) {
        case 0:
                pos.y--;
                return pos;
        case 1:
                pos.x++;
                return pos;
        case 2:
                pos.y++;
                return pos;
        case 3:
                pos.x--;
                return pos;
        default:
                return pos;
        }
}

void part1(struct input *input) {
        talint64 intcode = copyList(input->intcode);
        talpanel panels = tal_init();

        talint64 inputs = tal_init();
        talint64 outputs = tal_init();
        haltmode state = {0};
        ivec2 pos = {{0, 0}};
        int dir = 0; // 0: up, 1: right, 2: down, 3: left
        for (;;) {
                u64 posId = posToId(pos);
                bool color = findPanel(&panels, posId, -1);
                tal_add(inputs, color);
                state = runIntcode(&intcode, state, &inputs, &outputs);
                if (state.halt)
                        break;
                bool paint = tal_pop_front(outputs);
                findPanel(&panels, posId, paint);
                bool turn = tal_pop_front(outputs);
                dir += turn ? 1 : -1;
                dir = (dir + 4) % 4;
                pos = move(pos, dir);
        }

        int numPainted = panels.length;
        tal_destroy(panels);

        printf("Part 1: %d\n\n", numPainted);
}

void part2(struct input *input) {
        talint64 intcode = copyList(input->intcode);
        talpanel panels = tal_init();

        // Add initial white panel at position (0, 0)
        panel newPanel = {posToId((ivec2){{0, 0}}), true};
        tal_add(panels, newPanel);

        talint64 inputs = tal_init();
        talint64 outputs = tal_init();
        haltmode state = {0};
        ivec2 pos = {{0, 0}};
        int dir = 0; // 0: up, 1: right, 2: down, 3: left
        for (;;) {
                u64 posId = posToId(pos);
                bool color = findPanel(&panels, posId, -1);
                tal_add(inputs, color);
                state = runIntcode(&intcode, state, &inputs, &outputs);
                if (state.halt)
                        break;
                bool paint = tal_pop_front(outputs);
                findPanel(&panels, posId, paint);
                bool turn = tal_pop_front(outputs);
                dir += turn ? 1 : -1;
                dir = (dir + 4) % 4;
                pos = move(pos, dir);
        }

        // Find bounds of image
        ivec2 minPos = {{INT_MAX, INT_MAX}};
        ivec2 maxPos = {{INT_MIN, INT_MIN}};
        for (u64 i = 0; i < panels.length; i++) {
                ivec2 pos = idToPos(panels.array[i].posId);
                if (pos.x < minPos.x)
                        minPos.x = pos.x;
                if (pos.x > maxPos.x)
                        maxPos.x = pos.x;
                if (pos.y < minPos.y)
                        minPos.y = pos.y;
                if (pos.y > maxPos.y)
                        maxPos.y = pos.y;
        }

        printf("Part 2: \n");
        int index = 0;
        for (int y = minPos.y; y <= maxPos.y; y++) {
                for (int x = minPos.x; x <= maxPos.x; x++) {
                        panel p = panels.array[index];
                        ivec2 pos = idToPos(p.posId);
                        bool color = false;
                        if (pos.x == x && pos.y == y) {
                                color = p.color;
                                index++;
                        }
                        if (color)
                                printf("#");
                        else
                                printf(" ");
                }
                printf("\n");
        }
        tal_destroy(panels);
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
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day11.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        struct input input = parseInput(ll);
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

