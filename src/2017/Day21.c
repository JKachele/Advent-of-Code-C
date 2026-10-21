/*************************************************
 *File----------Day21.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Oct 21, 2026 12:39:03 EDT
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

#define INPUT_BUFFER_SIZE 1024

struct rule2 {
        int numOn;
        bool input[2][2];
        bool output[3][3];
};

struct rule3 {
        int numOn;
        bool input[3][3];
        bool output[4][4];
};

struct input {
        int numRule2;
        int numRule3;
        struct rule2 *rule2s;
        struct rule3 *rule3s;
};

static const bool START[3][3] = {{0, 1, 0},
                                 {0, 0, 1},
                                 {1, 1, 1}};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printRules(int numR2, int numR3, struct rule2 r2s[], struct rule3 r3s[]) {
        for (int i = 0; i < numR2; i++) {
                struct rule2 r = r2s[i];
                printf("%d\n", r.numOn);
                char lines[3][10];
                memset(lines, ' ', 30);
                for (int y = 0; y < 2; y++) {
                        for (int x = 0; x < 2; x++) {
                                lines[y][x] = r.input[y][x] ? '#' : '.';
                        }
                }
                for (int y = 0; y < 3; y++) {
                        lines[y][9] = '\0';
                        for (int x = 0; x < 3; x++) {
                                lines[y][x+6] = r.output[y][x] ? '#' : '.';
                        }
                }
                lines[1][3] = '-';
                lines[1][4] = '>';
                printf("%s\n%s\n%s\n\n", lines[0], lines[1], lines[2]);
        }

        for (int i = 0; i < numR3; i++) {
                struct rule3 r = r3s[i];
                printf("%d\n", r.numOn);
                char lines[4][12];
                memset(lines, ' ', 48);
                for (int y = 0; y < 3; y++) {
                        for (int x = 0; x < 3; x++) {
                                lines[y][x] = r.input[y][x] ? '#' : '.';
                        }
                }
                for (int y = 0; y < 4; y++) {
                        lines[y][11] = '\0';
                        for (int x = 0; x < 4; x++) {
                                lines[y][x+7] = r.output[y][x] ? '#' : '.';
                        }
                }
                lines[1][4] = '-';
                lines[1][5] = '>';
                printf("%s\n%s\n%s\n%s\n\n", lines[0], lines[1], lines[2], lines[3]);
        }
}

void printGrid(int size, bool grid[][size]) {
        for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                        printf("%c", grid[y][x] ? '#' : '.');
                }
                printf("\n");
        }
        printf("\n");
}

int findRule2(int size, const bool input[][size], int numR2, struct rule2 r2s[], int inx, int iny) {
        const ivec2 rotations[] = {{{0, 0}}, {{1, 0}}, {{1, 1}}, {{0, 1}}};

        bool in[2][2];
        int numOn = 0;
        for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 2; x++) {
                        in[y][x] = input[y + iny][x + inx];
                        if (in[y][x]) {
                                numOn++;
                        }
                }
        }

        // Dont need reflections for 2x2 input
        for (int i = 0; i < numR2; i++) {
                struct rule2 rule = r2s[i];
                if (numOn != rule.numOn) { continue; }

                // Check all 4 rotations of input grid
                for (int r = 0; r < 4; r++) {
                        bool found = true;
                        for (int j = 0; j < 4; j++) {
                                ivec2 rotate = rotations[(j + r) % 4];
                                ivec2 normal = rotations[j];
                                if (in[rotate.y][rotate.x] != rule.input[normal.y][normal.x]) {
                                        found = false;
                                        break;
                                }
                        }
                        // Return index of matching rule if found
                        if (found) {
                                return i;
                        }
                }
        }

        return -1;
}

bool **expand2(int size, const bool input[][size], int numR2, struct rule2 r2s[]) {
        int newSize = (size / 2) * 3;
        bool (*output)[newSize] = (bool(*)[newSize])malloc(newSize * newSize);

        for (int y = 0; y < size/2; y++) {
                for (int x = 0; x < size/2; x++) {
                        int ruleIndex = findRule2(size, input, numR2, r2s, x*2, y*2);
                        if (ruleIndex < 0) {
                                fprintf(stderr, "Rule Not Found!\n");
                                return NULL;
                        }

                        // Fill output with rule output
                        struct rule2 rule = r2s[ruleIndex];
                        for (int y1 = 0; y1 < 3; y1++) {
                                for (int x1 = 0; x1 < 3; x1++) {
                                        output[(y*3)+y1][(x*3)+x1] = rule.output[y1][x1];
                                }
                        }
                }
        }

        free((void*)input);
        return (bool**)output;
}

void flipGrid(bool grid[3][3]) {
        for (int y = 0; y < 3; y++) {
                bool temp = grid[y][0];
                grid[y][0] = grid[y][2];
                grid[y][2] = temp;
        }
}

// Rotate by Transposing (Flip along diagonal), the flipping horizontally
void rotateGrid(bool grid[3][3]) {
        for (int i = 0; i < 3; i++) {
                for (int j = i + 1; j < 3; j++) {
                        bool temp = grid[i][j];
                        grid[i][j] = grid[j][i];
                        grid[j][i] = temp;
                }
        }
        flipGrid(grid);
}

int findRule3(int size, const bool input[][size], int numR3, struct rule3 r3s[], int inx, int iny) {
        bool in[3][3];
        int numOn = 0;
        for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                        in[y][x] = input[y + iny][x + inx];
                        if (in[y][x]) {
                                numOn++;
                        }
                }
        }

        for (int i = 0; i < numR3; i++) {
                struct rule3 rule = r3s[i];
                if (numOn != rule.numOn) { continue; }

                // Center is always center
                if (in[1][1] != rule.input[1][1]) { continue; }

                for (int r = 0; r < 8; r++) {
                        bool found = true;
                        for (int y = 0; y < 3; y++) {
                                for (int x = 0; x < 3; x++) {
                                        if (in[y][x] != rule.input[y][x]) {
                                                found = false;
                                                break;
                                        }
                                }
                                if (!found) {
                                        break;
                                }
                        }
                        if (found) {
                                return i;
                        }
                        rotateGrid(in);
                        if ((r % 4) == 3) {
                                flipGrid(in);
                        }
                }
        }

        return -1;
}

bool **expand3(int size, const bool input[][size], int numR3, struct rule3 r3s[]) {
        int newSize = (size / 3) * 4;
        bool (*output)[newSize] = (bool(*)[newSize])malloc(newSize * newSize);

        for (int y = 0; y < size/3; y++) {
                for (int x = 0; x < size/3; x++) {
                        int ruleIndex = findRule3(size, input, numR3, r3s, x*3, y*3);
                        if (ruleIndex < 0) {
                                fprintf(stderr, "Rule Not Found!\n");
                                return NULL;
                        }

                        // Fill output with rule output
                        struct rule3 rule = r3s[ruleIndex];
                        for (int y1 = 0; y1 < 4; y1++) {
                                for (int x1 = 0; x1 < 4; x1++) {
                                        output[(y*4)+y1][(x*4)+x1] = rule.output[y1][x1];
                                }
                        }
                }
        }

        free((void*)input);
        return (bool**)output;
}

void part1(struct input *input) {
        const int NumItterations = 5;
        int numR2 = input->numRule2;
        int numR3 = input->numRule3;
        struct rule2 *r2s = input->rule2s;
        struct rule3 *r3s = input->rule3s;
        // printRules(numR2, numR3, r2s, r3s);

        int size = 3;
        bool **gridPtr = malloc(size * size);
        bool (*initGrid)[size] = (bool(*)[size])gridPtr;
        for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                        initGrid[y][x] = START[y][x];
                }
        }

        for (int i = 0; i < NumItterations; i++) {
                bool (*grid)[size] = (bool(*)[size])gridPtr;
                // printGrid(size, grid);
                if (size % 2 == 0) {
                        gridPtr = expand2(size, grid, numR3, r2s);
                        size = (size / 2) * 3;
                } else {
                        gridPtr = expand3(size, grid, numR3, r3s);
                        size = (size / 3) * 4;
                }
                if (gridPtr == NULL) {
                        return;
                }
        }
        bool (*grid)[size] = (bool(*)[size])gridPtr;
        // printGrid(size, grid);

        int numOn = 0;
        for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                        if (grid[y][x]) {
                                numOn++;
                        }
                }
        }

        printf("Part 1: Pixels on = %d\n\n", numOn);
}

void part2(struct input *input) {
        const int NumItterations = 18;
        int numR2 = input->numRule2;
        int numR3 = input->numRule3;
        struct rule2 *r2s = input->rule2s;
        struct rule3 *r3s = input->rule3s;
        // printRules(numR2, numR3, r2s, r3s);

        int size = 3;
        bool **gridPtr = malloc(size * size);
        bool (*initGrid)[size] = (bool(*)[size])gridPtr;
        for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                        initGrid[y][x] = START[y][x];
                }
        }

        for (int i = 0; i < NumItterations; i++) {
                bool (*grid)[size] = (bool(*)[size])gridPtr;
                // printGrid(size, grid);
                if (size % 2 == 0) {
                        gridPtr = expand2(size, grid, numR3, r2s);
                        size = (size / 2) * 3;
                } else {
                        gridPtr = expand3(size, grid, numR3, r3s);
                        size = (size / 3) * 4;
                }
                if (gridPtr == NULL) {
                        return;
                }
        }
        bool (*grid)[size] = (bool(*)[size])gridPtr;
        // printGrid(size, grid);

        int numOn = 0;
        for (int y = 0; y < size; y++) {
                for (int x = 0; x < size; x++) {
                        if (grid[y][x]) {
                                numOn++;
                        }
                }
        }

        printf("Part 2: Pixels on = %d\n", numOn);
}

struct input parseInput(llist *ll) {
        struct input input = {0};

        int numRule2 = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                if (strlen(str) > 20) {
                        break;
                }
                numRule2++;
        }

        input.numRule2 = numRule2;
        input.numRule3 = ll->length - numRule2;
        input.rule2s = malloc(input.numRule2 * sizeof(struct rule2));
        input.rule3s = malloc(input.numRule3 * sizeof(struct rule3));

        llNode *cur = ll->head;
        for (int i = 0; i < input.numRule2; i++) {
                char *str = (char*)cur->data;
                struct rule2 *r = &input.rule2s[i];

                int j = 0;
                r->numOn = 0;
                for (int y = 0; y < 2; y++) {
                        for (int x = 0; x < 2; x++) {
                                r->input[y][x] = (str[j++] == '#');
                                if (r->input[y][x]) {
                                        r->numOn++;
                                }
                        }
                        j++;
                }

                j = 9;
                for (int y = 0; y < 3; y++) {
                        for (int x = 0; x < 3; x++) {
                                r->output[y][x] = (str[j++] == '#');
                        }
                        j++;
                }

                cur = cur->next;
        }

        for (int i = 0; i < input.numRule3; i++) {
                char *str = (char*)cur->data;
                struct rule3 *r = &input.rule3s[i];

                int j = 0;
                r->numOn = 0;
                for (int y = 0; y < 3; y++) {
                        for (int x = 0; x < 3; x++) {
                                r->input[y][x] = (str[j++] == '#');
                                if (r->input[y][x]) {
                                        r->numOn++;
                                }
                        }
                        j++;
                }

                j = 15;
                for (int y = 0; y < 4; y++) {
                        for (int x = 0; x < 4; x++) {
                                r->output[y][x] = (str[j++] == '#');
                        }
                        j++;
                }

                cur = cur->next;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2017/Day21.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2017/Day21.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2017/Day21.txt";
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
        // printf("Execution Time (ms) - Input Parse: %f, Part1/2: %f\n", 
        //                 parseTime, pt1Time);

        return 0;
}

