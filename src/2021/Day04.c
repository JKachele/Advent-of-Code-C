/*************************************************
 *File----------Day04.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Sep 16, 2025 15:14:13 UTC
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

typedef tll(int) tllint;

typedef struct cell {
        int num;
        bool marked;
} cell;

typedef struct board {
        cell cells[5][5];
} board;
typedef tll(board) tllboard;

struct input {
        tllint draws;
        tllboard boards;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printBoard(board b) {
        for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                        printf("%02d ", b.cells[y][x].num);
                }
                printf("\n");
        }
        printf("\n");
}

void printBoards(tllboard boards) {
        tll_foreach(boards, it) {
                printBoard(it->item);
        }
}

void markBoard(board *b, int num) {
        for (int i=0; i<25; i++) {
                int x = i % 5;
                int y = i / 5;
                if (b->cells[y][x].num == num)
                        b->cells[y][x].marked = true;
        }
}

bool boardBingo(board b) {
        bool colBingo[5] = {1, 1, 1, 1, 1};
        for (int y=0; y<5; y++) {
                bool rowBingo = true;
                for (int x=0; x<5; x++) {
                        if (b.cells[y][x].marked == false) {
                                colBingo[x] = false;
                                rowBingo = false;
                        }
                }
                if (rowBingo)
                        return true;
        }
        for (int i=0; i<5; i++) {
                if (colBingo[i])
                        return true;
        }
        return false;
}

int32 boardScore(board b, int num) {
        int score = 0;
        for (int i=0; i<25; i++) {
                int x = i % 5;
                int y = i / 5;
                if (b.cells[y][x].marked == false)
                        score += b.cells[y][x].num;
        }
        score *= num;
        return score;
}

void part1(struct input input) {
        tllint draws = input.draws;
        tllboard boards = input.boards;

        board winner = {0};
        int winNum = 0;
        tll_foreach(draws, it) {
                int draw = it->item;
                bool winnerFound = false;
                tll_foreach(boards, bit) {
                        board *b = &bit->item;
                        markBoard(b, draw);
                        if (boardBingo(*b)) {
                                winner = *b;
                                winNum = draw;
                                winnerFound = true;
                                break;
                        }
                }
                if (winnerFound)
                        break;
        }

        int score = boardScore(winner, winNum);

        printf("Part 1: %d\n\n", score);
}

void part2(struct input input) {
        tllint draws = input.draws;
        tllboard boards = input.boards;

        board winner = {0};
        int winNum = 0;
        tll_foreach(draws, it) {
                int draw = it->item;
                tll_foreach(boards, bit) {
                        board *b = &bit->item;
                        markBoard(b, draw);
                        if (boardBingo(*b)) {
                                // Record each winning board
                                // Last board to win will renaim
                                winner = *b;
                                winNum = draw;
                                // Remove winning boards
                                tll_remove(boards, bit);
                        }
                }
                // Remove draws so the last draw is at the front
                tll_remove(draws, it);

                // Exit when no boards remain
                if (tll_length(boards) == 0) {
                        break;
                }
        }

        int score = boardScore(winner, winNum);

        printf("Part 2: %d\n\n", score);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        tllint draws = tll_init();
        tllboard boards = tll_init();

        llNode *current = ll->head;
        char *str = (char*)current->data;

        char *token = strtok(str, ",");
        while(token != NULL) {
                int draw = strtol(token, NULL, 10);
                tll_push_back(draws, draw);
                token = strtok(NULL, ",");
        }
        input.draws = draws;

        current = current->next->next;
        int y = 0;
        board curBoard = {0};
        while(current != NULL) {
                char *str = (char*)current->data;
                if (strlen(str) == 0) {
                        tll_push_back(boards, curBoard);
                        y = 0;
                        curBoard = (board){0};
                        current = current->next;
                        continue;
                }

                char *token = strtok(str, " ");
                for (int x=0; x<5; x++) {
                        int num = strtol(token, NULL, 10);
                        curBoard.cells[y][x].num = num;
                        token = strtok(NULL, " ");
                }
                current = current->next;
                y++;
        }
        // Push last board
        tll_push_back(boards, curBoard);
        // printBoards(boards);
        input.boards = boards;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2021/Day04.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2021/Day04.txt";
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

