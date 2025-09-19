/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Friday Sep 19, 2025 16:11:29 UTC
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
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(ivec2) tllivec2;

typedef struct fold {
        bool dir;       // true: Vert(x), false: Horiz(y)
        int32 val;
} fold;
typedef tll(fold) tllfold;

struct input {
        tllivec2 points;
        tllfold folds;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printSheet(tllivec2 points) {
        ivec2 size = {0};
        tll_foreach(points, it) {
                ivec2 p = it->item;
                if (p.x > size.x)
                        size.x = p.x;
                if (p.y > size.y)
                        size.y = p.y;
        }
        size.x++;
        size.y++;

        bool sheet[size.y][size.x];
        for (int i=0; i<size.x*size.y; i++)
                sheet[i/size.x][i%size.x] = false;
        tll_foreach(points, it) {
                ivec2 p = it->item;
                sheet[p.y][p.x] = true;
        }

        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (sheet[y][x])
                                printf("#");
                        else
                                printf(".");
                }
                printf("\n");
        }
        printf("\n");
}

void foldSheet(tllivec2 *points, fold f) {
        bool *seen = calloc(0x100000000, sizeof(bool));
        tll_foreach(*points, it) {
                ivec2 *p = &it->item;
                if (f.dir && p->x > f.val)
                        p->x = f.val - (p->x - f.val);
                else if (!f.dir && p->y > f.val)
                        p->y = f.val - (p->y - f.val);
                u32 seenIndex = (p->x << 16) + p->y;
                if (seen[seenIndex])
                        tll_remove(*points, it);
                else
                        seen[seenIndex] = true;
        }
}

void part1(struct input input) {
        foldSheet(&input.points, tll_front(input.folds));

        int32 numPoints = tll_length(input.points);

        printf("Part 1: %d\n\n", numPoints);
}

void part2(struct input input) {
        tllivec2 points = input.points;
        tllfold folds = input.folds;

        tll_foreach(folds, it) {
                foldSheet(&points, it->item);
        }

        printf("Part 2: \n");
        printSheet(points);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        tllivec2 *points = &input.points;
        tllfold *folds = &input.folds;

        llNode *current = ll->head;
        while(current != NULL) {
                char *str = (char*)current->data;
                if (strlen(str) == 0)
                        break;

                ivec2 pos;
                pos.x = strtol(strtok(str, ","), NULL, 10);
                pos.y = strtol(strtok(NULL, ""), NULL, 10);
                tll_push_back(*points, pos);

                current = current->next;
        }

        current = current->next;
        while(current != NULL) {
                char *str = (char*)current->data;

                // "fold along y=7"  dir at str[11], val at str+13
                fold f;
                f.dir = str[11] == 'x';
                f.val = strtol(str+13, NULL, 10);
                tll_push_back(*folds, f);

                current = current->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day13.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day13.txt";
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

