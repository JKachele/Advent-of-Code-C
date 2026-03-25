/*************************************************
 *File----------Day24.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Mar 25, 2026 16:29:47 UTC
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
#include "../lib/tllist.h"
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

typedef tll(ivec2) tllivec2;
typedef tll(u16) tllu16;

enum dir {
        EAST,
        SEAST,
        SWEST,
        WEST,
        NWEST,
        NEAST
};
typedef tll(enum dir) tlldir;
typedef tll(tlldir) tlltile;

struct input {
        tlltile tiles;
};

//                      East     S-East    S-West      West      N-West      N-East
const ivec2 DIRS[] = {{{1, 0}}, {{1, 1}}, {{0, 1}}, {{-1, 0}}, {{-1, -1}}, {{0, -1}}};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printTile(tlldir dirs) {
        tll_foreach(dirs, it) {
                switch (it->item) {
                case EAST:
                        printf("E ");
                        break;
                case SEAST:
                        printf("SE ");
                        break;
                case SWEST:
                        printf("SW ");
                        break;
                case WEST:
                        printf("W ");
                        break;
                case NWEST:
                        printf("NW ");
                        break;
                case NEAST:
                        printf("NE ");
                        break;
                }
        }
        printf("\n");
}

u16 postoid(ivec2 pos) {
        u16 id = pos.x + 127;
        id <<= 8;
        id += pos.y + 127;
        return id;
}

ivec2 idtopos(u16 id) {
        ivec2 pos = {0};
        pos.y = (int)(id & 0xff) - 127;
        id >>= 8;
        pos.x = (int)(id & 0xff) - 127;
        return pos;
}

ivec2 findTile(tlldir tile) {
        ivec2 curPos = {{0, 0}};
        tll_foreach(tile, it) {
                curPos = ivec2Add(curPos, DIRS[it->item]);
        }
        return curPos;
}

void flipTile(tllivec2 *blackTiles, ivec2 tile) {
        bool found = false;
        tll_foreach(*blackTiles, it) {
                if (ivec2Eq(it->item, tile)) {
                        found = true;
                        tll_remove(*blackTiles, it);
                        break;
                }
        }
        if (!found)
                tll_push_back(*blackTiles, tile);
}

bool validPos(ivec2 pos) {
        return pos.x >= -127 && pos.x <= 128 && pos.y >= -127 && pos.y <= 128;
}

void stepTiles(bool *blackTiles) {
        tllu16 changes = tll_init();
        for (int i = 0; i < 0x10000; i++) {
                ivec2 pos = idtopos(i);
                int numNeighbors = 0;
                for (int j = 0; j < 6; j++) {
                        ivec2 newPos = ivec2Add(pos, DIRS[j]);
                        if (!validPos(newPos)) continue;
                        if (blackTiles[postoid(newPos)])
                                numNeighbors++;
                }
                if ((blackTiles[i] && (numNeighbors == 0 || numNeighbors > 2)) ||
                                (!blackTiles[i] && numNeighbors == 2))
                        tll_push_back(changes, i);
        }

        // Flip changed tiles
        tll_foreach(changes, it)
                blackTiles[it->item] = !blackTiles[it->item];
        tll_free(changes);
}

int numBlackTiles(bool *blackTiles) {
        int num = 0;
        for (int i = 0; i < 0x10000; i++) {
                if (blackTiles[i])
                        num++;
        }
        return num;
}

void part1(struct input input) {
        tllivec2 blackTiles = tll_init();

        tll_foreach(input.tiles, it) {
                tlldir tileDirs = it->item;
                ivec2 tilePos = findTile(tileDirs);
                flipTile(&blackTiles, tilePos);
        }

        int numBlackTiles = tll_length(blackTiles);

        printf("Part 1: %d\n\n", numBlackTiles);
}

void part2(struct input input) {
        const int NumDays = 100;

        bool *blackTiles = calloc(0xffff, sizeof(bool));
        tll_foreach(input.tiles, it) {
                tlldir tileDirs = it->item;
                ivec2 tilePos = findTile(tileDirs);
                u16 id = postoid(tilePos);
                blackTiles[id] = !blackTiles[id];
        }

        for (int i = 0; i < NumDays; i++) {
                stepTiles(blackTiles);
                // printf("Day %d: %d\n", i+1, numBlackTiles(blackTiles));
        }

        int numBlack = numBlackTiles(blackTiles);

        printf("Part 2: %d\n", numBlack);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                tlldir dirs = tll_init();
                for (int i = 0; i < (int)strlen(str); i++) {
                        // 'e' and 'w' are always just east and west
                        // 'n' and 's' are always followed by 'e' or 'w'
                        if (str[i] == 'e') {
                                tll_push_back(dirs, EAST);
                        } else if (str[i] == 'w') {
                                tll_push_back(dirs, WEST);
                        } else if (str[i] == 'n') {
                                i++;
                                if (str[i] == 'e')
                                        tll_push_back(dirs, NEAST);
                                else if (str[i] == 'w')
                                        tll_push_back(dirs, NWEST);
                        } else if (str[i] == 's') {
                                i++;
                                if (str[i] == 'e')
                                        tll_push_back(dirs, SEAST);
                                else if (str[i] == 'w')
                                        tll_push_back(dirs, SWEST);
                        }
                }
                tll_push_back(input.tiles, dirs);
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day24.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day24.txt";
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

