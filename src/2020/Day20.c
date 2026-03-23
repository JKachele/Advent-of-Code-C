/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 23, 2026 18:02:50 UTC
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
#define TILE_SIZE 10

typedef enum dir {
        UP,
        RIGHT,
        DOWN,
        LEFT
} dir;

typedef struct image {
        int id;
        bool image[TILE_SIZE][TILE_SIZE];
        bool borders[4][TILE_SIZE];
} image;
typedef tll(image) tllimage;

typedef struct tile {
        int id;
        u16 borders[4];
        bool unique[4];
        bool corner;
        // NOTE: Transformations are always rotate THEN flip
        u8 transform; // Bits 0-1: Rotations, Bit 2: Flip
} tile;
typedef tll(tile) tlltile;

struct input {
        tllimage images;
        int numTiles;
        tile *tiles;
};

const int MonsterLen = 15;
const ivec2 Monster[] = {{{0, 0}}, {{1, 1}}, {{4, 1}}, {{5, 0}}, {{6, 0}}, {{7, 1}}, {{10, 1}},
        {{11, 0}}, {{12, 0}}, {{13, 1}}, {{16, 1}}, {{17, 0}}, {{18, 0}}, {{18, -1}}, {{19, 0}}};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printTile(image t) {
        printf("%d:\n", t.id);
        for (int y = 0; y < TILE_SIZE; y++) {
                for (int x = 0; x < TILE_SIZE; x++) {
                        printf("%c", t.image[y][x]? '#' : '.');
                }
                printf("\n");
        }
        printf("Borders:\n");
        for (int i = 0; i < 4; i++) {
                printf("%d: ", i);
                for (int j = 0; j < TILE_SIZE; j++) {
                        printf("%c", t.borders[i][j]? '#' : '.');
                }
                printf("\n");
        }
        printf("\n");
}

void printTiles(tllimage tiles, bool full) {
        tll_foreach(tiles, it) {
                image t = it->item;
                if (full)
                        printTile(t);
                else
                        printf("%d\n", t.id);
        }
}

void printBorder(u16 n) {
        const u16 Mask = 0x0200;
        for (int i = 0; i < TILE_SIZE; i++) {
                printf("%c", (n & Mask)? '#' : '.');
                n <<= 1;
        }
        printf("\n");
}

u16 arrtonum(bool arr[]) {
        u16 num = 0;
        for (int i = 0; i < TILE_SIZE; i++) {
                num <<= 1;
                num += (arr[i] ? 1 : 0);
        }
        return num;
}

tile imagetotile(image image) {
        tile t = {0};
        t.id = image.id;
        for (int i = 0; i < 4; i++)
                t.borders[i] = arrtonum(image.borders[i]);
        return t;
}

void copyTileArr(tile dest[], tile src[], int num) {
        for (int i = 0; i < num; i++)
                memcpy(&dest[i], &src[i], sizeof(tile));
}

u16 reverse(u16 n) {
        u16 rev = 0;
        for (int i = 0; i < TILE_SIZE; i++) {
                rev <<= 1;
                rev += n & 1;
                n >>= 1;
        }
        return rev;
}

void rotateTile(tile *t) {
        u16 temp = t->borders[LEFT];
        t->borders[LEFT] = t->borders[DOWN];
        t->borders[DOWN] = t->borders[RIGHT];
        t->borders[RIGHT] = t->borders[UP];
        t->borders[UP] = temp;
}

void flipTileVert(tile *t) {
        // Swap top and bottom borders
        u16 temp = t->borders[DOWN];
        t->borders[DOWN] = t->borders[UP];
        t->borders[UP] = temp;

        // Flip all borders
        for (int i = 0; i < 4; i++) {
                reverse(t->borders[i]);
        }
}

bool bordersMatch(u16 n, tile t) {
        for (int i = 0; i < 4; i++) {
                if (t.borders[i] == n)
                        return true;
        }
        return false;
}

void part1(struct input input) {
        int numTiles = tll_length(input.images);
        // Turn list of images to list of tiles
        tlltile tiles = tll_init();
        tll_foreach(input.images, it)
                tll_push_back(tiles, imagetotile(it->item));

        int64 ans = 1;
        // Find tiles with 2 unique borders
        tll_foreach(tiles, it) {
                tile *t = &it->item;
                int numUnique = 0;
                for (int b = 0; b < 4; b++) {
                        u16 border = t->borders[b];
                        bool unique = true;
                        tll_foreach(tiles, itt) {
                                tile t2 = itt->item;
                                if (t->id == t2.id) continue;
                                if (bordersMatch(border, t2)) {
                                        unique = false;
                                        break;
                                }       
                        }
                        if (!unique) continue;
                        border = reverse(border);
                        tll_foreach(tiles, itt) {
                                tile t2 = itt->item;
                                if (t->id == t2.id) continue;
                                if (bordersMatch(border, t2)) {
                                        unique = false;
                                        break;
                                }       
                        }
                        if (unique) {
                                numUnique++;
                                t->unique[b] = true;
                        }
                }
                if (numUnique == 2) {
                        t->corner = true;
                        ans *= t->id;
                }
        }

        printf("Part 1: %ld\n\n", ans);
}

void part2(struct input input) {
        printf("Part 2: \n");
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        int y = 0;
        image cur = {0};
        for (llNode *curLine = ll->head; curLine != NULL; curLine = curLine->next) {
                char *str = (char*)curLine->data;

                // Get tile ID
                if (str[0] == 'T') {
                        strtok(str, " ");
                        cur.id = strtol(strtok(NULL, ":"), NULL, 10);
                        y = 0;
                        continue;
                }

                // Add tile to list
                if (strlen(str) == 0) {
                        tll_push_back(input.images, cur);
                        cur = (image){0};
                        continue;
                }

                // Get tile contents
                for (int x = 0; x < TILE_SIZE; x++) {
                        cur.image[y][x] = (str[x] == '#');
                        if (y == 0)
                                cur.borders[UP][x] = (str[x] == '#');
                        if (x == TILE_SIZE-1)
                                cur.borders[RIGHT][y] = (str[x] == '#');
                        if (y == TILE_SIZE-1)
                                cur.borders[DOWN][(TILE_SIZE-1)-x] = (str[x] == '#');
                        if (x==0)
                                cur.borders[LEFT][(TILE_SIZE-1)-y] = (str[x] == '#');
                }
                y++;
        }
        tll_push_back(input.images, cur);

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day20.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day20.txt";
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

