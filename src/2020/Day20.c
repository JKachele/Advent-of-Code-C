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
#define IMAGE_SIZE 8

typedef enum dir {
        UP,
        RIGHT,
        DOWN,
        LEFT
} dir;

typedef struct image {
        int id;
        bool image[IMAGE_SIZE][IMAGE_SIZE];
        bool borders[4][TILE_SIZE];
} image;
typedef tll(image) tllimage;

typedef struct tile {
        int id;
        u16 borders[4];
        bool unique[4];
        int numUnique;
        // NOTE: Transformations are always rotate THEN flip
        u8 transform; // Bits 0-1: Rotations, Bit 2: Flip
} tile;
typedef tll(tile) tlltile;

struct input {
        tllimage images;
        tlltile tiles;
};

const int MonsterLen = 15;
const ivec2 MonsterSize = {{20, 3}};
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

void printImage(int size, bool image[][size]) {
        for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                        printf("%c", image[y][x]? '#' : '.');
                }
                printf("\n");
        }
        printf("\n");
}

void printTile(tile t) {
        const u16 Mask = 0x0200;
        u16 n = t.borders[UP];
        for (int i = 0; i < TILE_SIZE; i++) {
                printf("%c", (n & Mask)? '#' : '.');
                n <<= 1;
        }
        printf("\n");
        n = t.borders[LEFT] >> 1;
        u16 n2 = t.borders[RIGHT] << 1;
        for (int i = 1; i < TILE_SIZE - 1; i++) {
                printf("%c", (n & 1)? '#' : '.');
                printf("        ");
                printf("%c\n", (n2 & Mask)? '#' : '.');
                n >>= 1;
                n2 <<= 1;
        }
        n = t.borders[DOWN];
        for (int i = 0; i < TILE_SIZE; i++) {
                printf("%c", (n & 1)? '#' : '.');
                n >>= 1;
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

u16 reverse(u16 n) {
        u16 rev = 0;
        for (int i = 0; i < TILE_SIZE; i++) {
                rev <<= 1;
                rev += n & 1;
                n >>= 1;
        }
        return rev;
}

void rotateTile(tile *t, int n) {
        for (int i = 0; i < n; i++) {
                u16 temp = t->borders[LEFT];
                t->borders[LEFT] = t->borders[DOWN];
                t->borders[DOWN] = t->borders[RIGHT];
                t->borders[RIGHT] = t->borders[UP];
                t->borders[UP] = temp;
                bool tempU = t->unique[LEFT];
                t->unique[LEFT] = t->unique[DOWN];
                t->unique[DOWN] = t->unique[RIGHT];
                t->unique[RIGHT] = t->unique[UP];
                t->unique[UP] = tempU;
        }
}

void flipTileVert(tile *t) {
        // Swap top and bottom borders
        u16 temp = t->borders[DOWN];
        t->borders[DOWN] = t->borders[UP];
        t->borders[UP] = temp;
        bool tempU = t->unique[DOWN];
        t->unique[DOWN] = t->unique[UP];
        t->unique[UP] = tempU;

        // Flip all borders
        for (int i = 0; i < 4; i++) {
                t->borders[i] = reverse(t->borders[i]);
        }
}

int bordersMatch(u16 n, tile t) {
        // Return -1 for no match, 1-3 for match, and 4-7 for flip match
        for (int i = 0; i < 4; i++) {
                if (t.borders[i] == n)
                        return i;
        }
        n = reverse(n);
        for (int i = 0; i < 4; i++) {
                if (t.borders[i] == n)
                        return i + 4;
        }
        return -1;
}

void orientCorner(tile *t) {
        // make up and left borders unique
        for (int i = 0; i < 3; i++) {
                if (t->unique[UP] && t->unique[LEFT])
                        break;
                rotateTile(t, 1);
                t->transform++;
        }
}

int fillGridRow(tlltile *tiles, int gridSize, tile grid[][gridSize], ivec2 start) {
        ivec2 curPos = start;
        ivec2 nextPos = start;
        nextPos.x++;

        bool topRow = curPos.y == 0;

        // Fill row until right border is unique (edge)
        while (tll_length(*tiles) > 0) {
                tile cur = grid[curPos.y][curPos.x];
                if (cur.unique[RIGHT])
                        break;
                tile up = {0};
                if (!topRow)
                        up = grid[nextPos.y - 1][nextPos.x];

                u16 border = cur.borders[RIGHT];
                u16 borderUp = up.borders[DOWN];
                bool found = false;
                tll_foreach(*tiles, it) {
                        tile t2 = it->item;
                        int transform = bordersMatch(border, t2);
                        int transformUp = bordersMatch(borderUp, t2);
                        if (topRow) transformUp = 0; // Dont care about up border for top row
                        if (transform != -1 && transformUp != -1) {
                                // Rotate and flip tile to match
                                int numFlips = 3 - (transform % 4);
                                rotateTile(&t2, numFlips);
                                bool flipped = false;
                                if (transform < 4) {
                                        flipTileVert(&t2);
                                        flipped = true;
                                }
                                t2.transform = numFlips + (flipped ? 4 : 0);

                                // Add tile to grid and remove from list
                                // printf("Add tile %d at (%d, %d) ", t2.id, nextPos.x, nextPos.y);
                                // printf("with transform %d\n", t2.transform);
                                memcpy(&grid[nextPos.y][nextPos.x], &t2, sizeof(tile));
                                tll_remove(*tiles, it);
                                found = true;
                                break;
                        }
                }
                if (!found) {
                        printf("ERROR: No matching tile found for right border\n");
                        break;
                }
                // printTile(grid[nextPos.y][nextPos.x]);
                curPos = nextPos;
                nextPos.x++;
        }
        return nextPos.x;
}

ivec2 fillGrid(tlltile *tiles, int maxGridSize, tile grid[][maxGridSize]) {
        ivec2 curPos = {{0, 0}};
        ivec2 nextPos = {{0, 1}};

        // Fill first column
        while (tll_length(*tiles) > 0) {
                tile cur = grid[curPos.y][curPos.x];
                if (cur.unique[DOWN])
                        break;

                u16 border = cur.borders[DOWN];
                bool found = false;
                tll_foreach(*tiles, it) {
                        tile t2 = it->item;
                        int transform = bordersMatch(border, t2);
                        if (transform != -1) {
                                // Rotate and flip tile to match
                                int numFlips = (4 - (transform % 4)) % 4;
                                rotateTile(&t2, numFlips);
                                // If flipped, need to rotate twice then flip
                                bool flipped = false;
                                if (transform < 4) {
                                        rotateTile(&t2, 2);
                                        flipTileVert(&t2);
                                        numFlips = (numFlips + 2) % 4;
                                        flipped = true;
                                }
                                t2.transform = numFlips + (flipped ? 4 : 0);

                                // Tile cannot have a unique right border
                                // and must have a unique left border
                                if (t2.unique[RIGHT] || !t2.unique[LEFT])
                                        continue;

                                // Add tile to grid and remove from list
                                // printf("Add tile %d at (%d, %d) ", t2.id, nextPos.x, nextPos.y);
                                // printf("with transform %d\n", t2.transform);
                                memcpy(&grid[nextPos.y][nextPos.x], &t2, sizeof(tile));
                                tll_remove(*tiles, it);
                                found = true;
                                break;
                        }
                }
                if (!found) {
                        printf("ERROR: No matching tile found for down border\n");
                        break;
                }
                // printTile(grid[nextPos.y][nextPos.x]);
                curPos = nextPos;
                nextPos.y++;
        }

        // Fill all rows
        curPos = (ivec2){{0, 0}};
        ivec2 gridSize = {{0, nextPos.y}};
        while (tll_length(*tiles) > 0 && curPos.y < nextPos.y) {
                gridSize.x = fillGridRow(tiles, maxGridSize, grid, curPos);
                curPos.y++;
        }
        return gridSize;
}

void rotateImage(int size, bool image[][size], int n) {
        for (int i = 0; i < n; i++) {
                // Transpose image
                for (int j = 0; j < size; j++) {
                        for (int k = j + 1; k < size; k++) {
                                bool temp = image[j][k];
                                image[j][k] = image[k][j];
                                image[k][j] = temp;
                        }
                }
                // Flip each row
                for (int j = 0; j < size; j++) {
                        int a = 0;
                        int b = size - 1;
                        while (a < b) {
                                bool temp = image[j][a];
                                image[j][a] = image[j][b];
                                image[j][b] = temp;
                                a++;
                                b--;
                        }
                }
        }
}

void flipImage(int size, bool image[][size]) {
        // Flip each column
        for (int i = 0; i < size; i++) {
                int a = 0;
                int b = size - 1;
                while (a < b) {
                        bool temp = image[a][i];
                        image[a][i] = image[b][i];
                        image[b][i] = temp;
                        a++;
                        b--;
                }
        }
}

void fillImage(tllimage images, ivec2 size, bool fullImage[][size.x*8],
                int gridSize, tile grid[][gridSize]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        tile t = grid[y][x];

                        // Find image for tile
                        image img = {0};
                        tll_foreach(images, it) {
                                if (it->item.id == t.id) {
                                        memcpy(&img, &it->item, sizeof(image));
                                        tll_remove(images, it);
                                        break;
                                }
                        }

                        // Rotate and flip image to match tile
                        rotateImage(IMAGE_SIZE, img.image, t.transform % 4);
                        if (t.transform > 3)
                                flipImage(IMAGE_SIZE, img.image);

                        // Add image to full image
                        for (int i = 0; i < IMAGE_SIZE; i++) {
                                for (int j = 0; j < IMAGE_SIZE; j++) {
                                        ivec2 pos = {{x*8 + j, y*8 + i}};
                                        fullImage[pos.y][pos.x] = img.image[i][j];
                                }
                        }

                }
        }
}

bool isMonster(int y, int x, ivec2 size, bool fullImage[][size.x]) {
        // Check the segments of the monster
        for (int i = 0; i < MonsterLen; i++) {
                int newX = x + Monster[i].x;
                int newY = y + Monster[i].y;
                // If a segment isn't filled, return false
                if (!fullImage[newY][newX])
                        return false;
        }
        return true;
}

bool findMonsters(ivec2 size, bool fullImage[][size.x]) {
        bool monsterFound = false;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        // Monster needs 1 row above and below and 19 columns to the right to fit
                        if (y == 0 || y == size.y - 1 || x > size.x - 20)
                                continue;
                        // Check each filled space for monster pattern
                        if (fullImage[y][x] && isMonster(y, x, size, fullImage)) {
                                // If a monster was found, flip all spaces used by monster
                                monsterFound = true;
                                for (int i = 0; i < MonsterLen; i++) {
                                        int newX = x + Monster[i].x;
                                        int newY = y + Monster[i].y;
                                        fullImage[newY][newX] = false;
                                }
                        }
                }
        }
        return monsterFound;
}

void part1(struct input *input) {
        int numTiles = tll_length(input->images);
        // Turn list of images to list of tiles
        tlltile tiles = tll_init();
        tll_foreach(input->images, it)
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
                                if (bordersMatch(border, t2) != -1) {
                                        unique = false;
                                        break;
                                }       
                        }
                        if (!unique) continue;
                        border = reverse(border);
                        tll_foreach(tiles, itt) {
                                tile t2 = itt->item;
                                if (t->id == t2.id) continue;
                                if (bordersMatch(border, t2) != -1) {
                                        unique = false;
                                        break;
                                }       
                        }
                        if (unique) {
                                numUnique++;
                                t->unique[b] = true;
                        }
                }
                t->numUnique = numUnique;
                if (numUnique == 2) {
                        ans *= t->id;
                }
        }
        input->tiles = tiles;

        printf("Part 1: %ld\n\n", ans);
}

void part2(struct input *input) {
        int numTiles = tll_length(input->tiles);

        // Max side length is numTiles / 2;
        int maxLen = numTiles / 2;
        tile grid[maxLen][maxLen];
        for (int y = 0; y < maxLen; y++)
                for (int x = 0; x < maxLen; x++)
                        grid[y][x] = (tile){0};

        // Put a corner tile at 0,0 and orient it correctly (up and left are unique)
        tll_foreach(input->tiles, it) {
                if (it->item.numUnique == 2) {
                        memcpy(&grid[0][0], &it->item, sizeof(tile));
                        tll_remove(input->tiles, it);
                        break;
                }
        }
        orientCorner(&grid[0][0]);
        // printTile(grid[0][0]);
        ivec2 size = fillGrid(&input->tiles, maxLen, grid);

        // Tile images are 8x8
        bool fullImage[size.y*8][size.x*8];
        fillImage(input->images, size, fullImage, maxLen, grid);
        // printImage(size.x*8, fullImage);

        // Check all rotations and flips for monsters
        size = (ivec2){{size.x*8, size.y*8}};
        for (int i = 0; i < 4; i++) {
                if (findMonsters(size, fullImage))
                        break;
                flipImage(size.x, fullImage);
                if (findMonsters(size, fullImage))
                        break;
                flipImage(size.x, fullImage);
                rotateImage(size.x, fullImage, 1);
        }
        // printImage(size.x, fullImage);

        // Count remaining cells
        int numCellsLeft = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (fullImage[y][x])
                                numCellsLeft++;
                }
        }

        printf("Part 2: %d\n", numCellsLeft);
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
                        if (x != 0 && y != 0 && x != TILE_SIZE-1 && y != TILE_SIZE-1)
                                cur.image[y-1][x-1] = (str[x] == '#');

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

