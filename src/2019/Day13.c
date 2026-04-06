/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Apr 06, 2026 21:31:50 UTC
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
// #include "../lib/tllist.h"

#define INPUT_BUFFER_SIZE 16384

typedef enum tile {
        EMPTY,
        WALL,
        BLOCK,
        PADDLE,
        BALL
} tile;

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

void part1(struct input *input) {
        talint64 intcode = copyList(input->intcode);

        talint64 outputs = tal_init();
        runIntcode(&intcode, (haltmode){0}, NULL, &outputs);

        int64 numBlocks = 0;
        for (int i = 0; i < (int)outputs.length; i += 3) {
                if (outputs.array[i + 2] == 2)
                        numBlocks++;
        }

        printf("Part 1: %ld\n\n", numBlocks);
}

void part2(struct input *input) {
        talint64 intcode = copyList(input->intcode);
        intcode.array[0] = 2;

        talint64 inputs = tal_init();
        talint64 outputs = tal_init();
        haltmode state = {0};
        int64 score = 0;
        do{
                state = runIntcode(&intcode, state, &inputs, &outputs);

                int64 ballPos = 0;
                int64 paddlePos = 0;
                for (int i = 0; i < (int)outputs.length; i += 3) {
                        if (outputs.array[i] == -1)
                                score = outputs.array[i+2];
                        else if (outputs.array[i+2] == BALL)
                                ballPos = outputs.array[i];
                        else if (outputs.array[i+2] == PADDLE)
                                paddlePos = outputs.array[i];
                }
                tal_destroy(outputs); // Clear outputs

                // Set joystick
                int64 joystick = (ballPos > paddlePos) ? 1 : ((ballPos < paddlePos) ? -1 : 0);
                tal_add(inputs, joystick);
        } while (!state.halt);

        printf("Part 2: %ld\n", score);
}

void part2Test(struct input *input) {
        talint64 intcode = copyList(input->intcode);
        intcode.array[0] = 2;

        talint64 inputs = tal_init();
        talint64 outputs = tal_init();
        runIntcode(&intcode, (haltmode){0}, &inputs, &outputs);

        ivec2 size = {0};
        for (int i = 0; i < (int)outputs.length; i += 3) {
                printf("(%ld, %ld, %ld)\n",
                                outputs.array[i], outputs.array[i + 1], outputs.array[i + 2]);
                if (outputs.array[i] > size.x)
                        size.x = outputs.array[i];
                if (outputs.array[i+1] > size.y)
                        size.y = outputs.array[i+1];
        }
        size.x++;
        size.y++;

        tile board[size.y][size.x];
        for (int i = 0; i < (int)outputs.length; i += 3) {
                if (outputs.array[i] == -1)
                        continue;
                int x = outputs.array[i];
                int y = outputs.array[i + 1];
                board[y][x] = (tile)outputs.array[i + 2];
        }

        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        switch (board[y][x]) {
                        case EMPTY:
                                printf(" ");
                                break;
                        case WALL:
                                printf("#");
                                break;
                        case BLOCK:
                                printf("X");
                                break;
                        case PADDLE:
                                printf("_");
                                break;
                        case BALL:
                                printf("O");
                                break;
                        }
                }
                printf("\n");
        }

        printf("Part 2: \n");
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
                char *file = "assets/tests/2019/Day13.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day13.txt";
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

