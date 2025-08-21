/*************************************************
 *File----------Day22.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Aug 20, 2025 15:11:32 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define INPUT_BUFFER_SIZE 8192

enum celltype {
        EMPTY,
        OPEN,
        WALL
};

enum facing {
        RIGHT = 0,
        DOWN = 1,
        LEFT = 2,
        UP = 3
};

typedef struct {
        enum celltype type;
        // 0: Right, 1: Down, 2: Left, 3: Up
        ivec2 nextCells[4];     // list of the next cells in the direction
} boardCell;

struct move {
        bool type; // True: move, False: turn
        int32 move;
        bool dir; // True: Right, False: Left
};

typedef tll(struct move) tllmove;

typedef struct {
        int32 x;
        int32 y;
        enum facing dir;
} position;

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printBoard(ivec2 dim, boardCell board[dim.y][dim.x]) {
        for (int y=0; y<dim.y; y++) {
                for (int x=0; x<dim.x; x++) {
                        switch (board[y][x].type) {
                        case EMPTY:
                                printf(".");
                                break;
                        case OPEN:
                                printf("O");
                                break;
                        case WALL:
                                printf("#");
                                break;
                        }
                }
                printf("\n");
        }
        printf("\n");
}

void printMoves(tllmove moves) {
        tll_foreach(moves, it) {
                struct move m = it->item;
                if (m.type)
                        printf("%d\n", m.move);
                else
                        printf("%s\n", m.dir? "Right" : "Left");
        }
}

void analyzeCell(ivec2 dim, boardCell board[dim.y][dim.x], int x, int y) {
        const ivec2 dirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        if (board[y][x].type == EMPTY) return;

        for (int i=0; i<4; i++) {
                ivec2 new = {x + dirs[i].x, y + dirs[i].y};
                if (board[new.y][new.x].type != EMPTY) {
                        board[y][x].nextCells[i] = new;
                        continue;
                }
                // If next cell is empty, travel in the
                // opposite direction until an empty cell
                ivec2 opp = dirs[(i+2)%4];
                ivec2 cur = {x, y};
                while(board[cur.y+opp.y][cur.x+opp.x].type != EMPTY) {
                        cur.x += opp.x;
                        cur.y += opp.y;
                }
                board[y][x].nextCells[i] = cur;
        }
}

void analyzeBoard(ivec2 dim, boardCell board[dim.y][dim.x]) {
        // No need to go over the cells on the border
        for (int y=1; y<dim.y - 1; y++) {
                for (int x=1; x<dim.x - 1; x++) {
                        analyzeCell(dim, board, x, y);
                }
        }
}

void parseMoves(tllmove *moves, char *input) {
        int32 len = strlen(input);

        int32 curNum = 0;
        for (int i=0; i<len; i++) {
                char c = input[i];
                if (isDigit(c)) {
                        curNum = curNum * 10 + (c - '0');
                        continue;
                }
                if (curNum > 0) {
                        struct move m = {true, curNum, 0};
                        tll_push_back(*moves, m);
                        curNum = 0;
                }

                struct move mTurn = {false, 0, 0};
                switch (c) {
                case 'R':
                        mTurn.dir = true;
                        break;
                case 'L':
                        mTurn.dir = false;
                        break;
                default:
                        printf("Invalid move: %c\n", c);
                }
                tll_push_back(*moves, mTurn);
        }
        if (curNum > 0) {
                struct move m = {true, curNum, 0};
                tll_push_back(*moves, m);
        }
}

position walkPath(ivec2 dim, boardCell board[dim.y][dim.x],
                tllmove moves, ivec2 start) {
        const ivec2 dirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        position cur = {start.x, start.y, RIGHT};

        tll_foreach(moves, it) {
                struct move m = it->item;

                // if turn
                if (!m.type) {
                        // Turn Right
                        if (m.dir) {
                                cur.dir = (cur.dir + 1) % 4;
                        } else {
                                cur.dir = (cur.dir + 3) % 4;
                        }
                        continue;
                }

                // if move
                for (int i=0; i<m.move; i++) {
                        // printf("(%d, %d) %d\n", cur.x, cur.y, cur.dir);
                        ivec2 new = board[cur.y][cur.x].nextCells[cur.dir];
                        if (board[new.y][new.x].type == WALL)
                                break;
                        cur.x = new.x;
                        cur.y = new.y;
                }
        }

        return cur;
}

void part1(llist *ll) {
        ivec2 mapDim = {0, 0};

        // Get x dimension of map
        llNode *current = ll->head;
        while(current != NULL) {
                int32 len = strlen((char*)current->data);
                if (len == 0) break;
                if (len > mapDim.x)
                        mapDim.x = len;
                current = current->next;
                mapDim.y++;
        }
        // Add border around board of empty cells
        mapDim.x += 2;
        mapDim.y += 2;
        // printf("(%d, %d)\n", mapDim.x, mapDim.y);

        boardCell board[mapDim.y][mapDim.x];
        memset(board, 0, (mapDim.x * mapDim.y) * sizeof(boardCell));
        ivec2 startPos = {0, 0};
        
        // Populate board
        current = ll->head;
        int32 index = 1;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                int32 len = strlen(str);
                if (len == 0) break;

                for (int i=1; i<len + 1; i++) {
                        switch (str[i - 1]) {
                        case ' ':
                                board[index][i].type = EMPTY;
                                break;
                        case '.':
                                board[index][i].type = OPEN;
                                if (startPos.x == 0 && startPos.y == 0)
                                        startPos = (ivec2){i, index};
                                break;
                        case '#':
                                board[index][i].type = WALL;
                                if (startPos.x == 0 && startPos.y == 0)
                                        startPos = (ivec2){i, index};
                                break;
                        default:
                                printf("Unknown character at (%d, %d): %c\n",
                                                i, index, str[i]);
                        }
                }
                current = current->next;
                index++;
        }
        analyzeBoard(mapDim, board);
        // printBoard(mapDim, board);

        tllmove moves = tll_init();
        char moveStr[INPUT_BUFFER_SIZE];
        strncpy(moveStr, (char*)ll->tail->data, INPUT_BUFFER_SIZE);
        parseMoves(&moves, moveStr);
        // printMoves(moves);

        position final = walkPath(mapDim, board, moves, startPos);
        int32 passwd = (1000 * final.y) + (4 * final.x) + final.dir;

        printf("Part 1: Password: %d\n", passwd);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2022/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2022/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1(ll);
        clock_t pt1 = clock();
        part2(ll);
        clock_t pt2 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        double pt2Time = ((double)(pt2 - pt1) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Part1: %f, Part2: %f\n", 
                        parseTime, pt1Time, pt2Time);

        return 0;
}

