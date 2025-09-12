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
#include "../util/vector.h"

#define INPUT_BUFFER_SIZE 8192

int CubeSize = 50;
int CubeIndex = 0;

enum celltype {
        EMPTY,
        OPEN,
        WALL
};

typedef enum facing {
        RIGHT = 0,
        DOWN = 1,
        LEFT = 2,
        UP = 3,
        NONE = -1
} facing;

typedef struct {
        int32 x;
        int32 y;
        enum facing dir;
} position;
typedef tll(position) tllpos;

typedef struct {
        enum celltype type;
        // 0: Right, 1: Down, 2: Left, 3: Up
        position nextCells[4];     // list of the next cells in the direction
        facing lastDir;
} boardCell;

struct move {
        bool type; // True: move, False: turn
        int32 move;
        bool dir; // True: Right, False: Left
};

typedef tll(struct move) tllmove;

typedef struct {
        ivec2 pos;
        facing edge;
        ivec2 dest;
        facing dir;
} edge;

const edge edgeMap[2][14] = {{
        // Real Input
        {{{1, 0}}, UP, {{0, 3}}, RIGHT},
        {{{1, 0}}, LEFT, {{0, 2}}, RIGHT},

        {{{2, 0}}, UP, {{0, 3}}, UP},
        {{{2, 0}}, RIGHT, {{1, 2}}, LEFT},
        {{{2, 0}}, DOWN, {{1, 1}}, LEFT},

        {{{1, 1}}, RIGHT, {{2, 0}}, UP},
        {{{1, 1}}, LEFT, {{0, 2}}, DOWN},

        {{{0, 2}}, UP, {{1, 1}}, RIGHT},
        {{{0, 2}}, LEFT, {{1, 0}}, RIGHT},

        {{{1, 2}}, RIGHT, {{2, 0}}, LEFT},
        {{{1, 2}}, DOWN, {{0, 3}}, LEFT},

        {{{0, 3}}, RIGHT, {{1, 2}}, UP},
        {{{0, 3}}, DOWN, {{2, 0}}, DOWN},
        {{{0, 3}}, LEFT, {{1, 0}}, DOWN},
},{
        // Test Input
        {{{2, 0}}, UP, {{0, 1}}, DOWN},
        {{{2, 0}}, RIGHT, {{3, 2}}, LEFT},
        {{{2, 0}}, LEFT, {{1, 1}}, DOWN},

        {{{0, 1}}, UP, {{2, 0}}, DOWN},
        {{{0, 1}}, LEFT, {{3, 2}}, UP},
        {{{0, 1}}, DOWN, {{2, 2}}, UP},

        {{{1, 1}}, UP, {{2, 0}}, RIGHT},
        {{{1, 1}}, DOWN, {{2, 2}}, RIGHT},

        {{{2, 1}}, RIGHT, {{3, 2}}, DOWN},

        {{{2, 2}}, LEFT, {{1, 1}}, UP},
        {{{2, 2}}, DOWN, {{0, 1}}, UP},

        {{{3, 2}}, UP, {{2, 1}}, LEFT},
        {{{3, 2}}, RIGHT, {{2, 0}}, LEFT},
        {{{3, 2}}, DOWN, {{0, 1}}, RIGHT},
}};

static const ivec2 DIRS[4] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

char dirtochar(facing dir) {
        switch (dir) {
        case UP:
                return '^';
        case RIGHT:
                return '>';
        case DOWN:
                return 'V';
        case LEFT:
                return '<';
        default:
                return '?';
        }
}

void printBoard(ivec2 dim, boardCell board[dim.y][dim.x]) {
        for (int y=0; y<dim.y; y++) {
                for (int x=0; x<dim.x; x++) {
                        if (board[y][x].lastDir != NONE) {
                                printf("%c", dirtochar(board[y][x].lastDir));
                                continue;
                        }
                        switch (board[y][x].type) {
                                case EMPTY:
                                        printf(" ");
                                        break;
                                case OPEN:
                                        printf(".");
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
        if (board[y][x].type == EMPTY) return;

        for (int i=0; i<4; i++) {
                position new = {x + DIRS[i].x, y + DIRS[i].y, NONE};
                if (board[new.y][new.x].type != EMPTY) {
                        board[y][x].nextCells[i] = new;
                        continue;
                }
                // If next cell is empty, travel in the
                // opposite direction until an empty cell
                ivec2 opp = DIRS[(i+2)%4];
                position cur = {x, y, NONE};
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
        // Set lastDir to NONE
        for (int y=0; y<dim.y; y++) {
                for (int x=0; x<dim.x; x++) {
                        board[y][x].lastDir = NONE;
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
        position cur = {start.x, start.y, RIGHT};
        board[start.y][start.x].lastDir = cur.dir;

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
                        board[cur.y][cur.x].lastDir = cur.dir;
                        continue;
                }

                // if move
                for (int i=0; i<m.move; i++) {
                        // printf("(%d, %d) %d\n", cur.x, cur.y, cur.dir);
                        position new = board[cur.y][cur.x].nextCells[cur.dir];
                        if (board[new.y][new.x].type == WALL)
                                break;
                        cur.x = new.x;
                        cur.y = new.y;
                        if (new.dir != NONE)
                                cur.dir = new.dir;
                        board[cur.y][cur.x].lastDir = cur.dir;
                }
        }

        return cur;
}

position getNextSide(int x, int y, facing dir) {
        position new = {0};
        for (int i=0; i<14; i++) {
                edge e = edgeMap[CubeIndex][i];
                if (e.pos.x == x && e.pos.y == y && e.edge == dir) {
                        new.x = e.dest.x;
                        new.y = e.dest.y;
                        new.dir = e.dir;
                        goto exit;
                }
        }
        printf("Edge not found for (%d, %d, %d)\n", x, y, dir);
exit:
        return new;
}

ivec2 getFullPos(position old, position newSide) {
        int32 sideIndex = 0; // Side index is index clockwise from corner
        ivec2 adjPos = {{(old.x - 1) % CubeSize, (old.y - 1) % CubeSize}};
        switch (old.dir) {
        case UP:
                sideIndex = adjPos.x;
                break;
        case RIGHT:
                sideIndex = adjPos.y;
                break;
        case DOWN:
                sideIndex = CubeSize - adjPos.x - 1;
                break;
        case LEFT:
                sideIndex = CubeSize - adjPos.y - 1;
                break;
        default:
                printf("Error: Invalid direction\n");
                break;
        }

        ivec2 newAdjPos = {0};
        switch (newSide.dir) {
        case UP:
                newAdjPos.x = sideIndex;
                newAdjPos.y = CubeSize - 1;
                break;
        case RIGHT:
                newAdjPos.x = 0;
                newAdjPos.y = sideIndex;
                break;
        case DOWN:
                newAdjPos.x = CubeSize - sideIndex - 1;
                newAdjPos.y = 0;
                break;
        case LEFT:
                newAdjPos.x = CubeSize - 1;
                newAdjPos.y = CubeSize - sideIndex - 1;
                break;
        default:
                printf("Error: Invalid direction\n");
                break;
        }

        ivec2 newPos;
        newPos.x = 1 + (CubeSize * newSide.x) + newAdjPos.x;
        newPos.y = 1 + (CubeSize * newSide.y) + newAdjPos.y;

        return newPos;
}

position getNextPos(int x, int y, facing dir) {
        position new = {0};

        int sidex = (x - 1) / CubeSize;
        int sidey = (y - 1) / CubeSize;
        position newSide = getNextSide(sidex, sidey, dir);

        new.dir = newSide.dir;
        ivec2 newPos = getFullPos((position){x, y, dir}, newSide);
        new.x = newPos.x;
        new.y = newPos.y;

        return new;
}

void analyzeCellCube(ivec2 dim, boardCell board[dim.y][dim.x], int x, int y) {
        if (board[y][x].type == EMPTY) return;

        for (int i=0; i<4; i++) {
                position new = {x + DIRS[i].x, y + DIRS[i].y, NONE};
                if (board[new.y][new.x].type != EMPTY) {
                        board[y][x].nextCells[i] = new;
                        continue;
                }
                // If next cell is empty, travel in the
                // opposite direction until an empty cell
                new = getNextPos(x, y, i);
                board[y][x].nextCells[i] = new;
        }
}

void analyzeBoardCube(ivec2 dim, boardCell board[dim.y][dim.x]) {
        // No need to go over the cells on the border
        for (int y=1; y<dim.y - 1; y++) {
                for (int x=1; x<dim.x - 1; x++) {
                        analyzeCellCube(dim, board, x, y);
                }
        }
        // Set lastDir to NONE
        for (int y=0; y<dim.y; y++) {
                for (int x=0; x<dim.x; x++) {
                        board[y][x].lastDir = NONE;
                }
        }
}

void part1(llist *ll) {
        ivec2 mapDim = {0};

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
        ivec2 startPos = {0};
        
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
                                        startPos = (ivec2){{i, index}};
                                break;
                        case '#':
                                board[index][i].type = WALL;
                                if (startPos.x == 0 && startPos.y == 0)
                                        startPos = (ivec2){{i, index}};
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

// TODO: General Solution
void part2(llist *ll) {
        ivec2 mapDim = {0};

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
        ivec2 startPos = {0};
        
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
                                        startPos = (ivec2){{i, index}};
                                break;
                        case '#':
                                board[index][i].type = WALL;
                                if (startPos.x == 0 && startPos.y == 0)
                                        startPos = (ivec2){{i, index}};
                                break;
                        default:
                                printf("Unknown character at (%d, %d): %c\n",
                                                i, index, str[i]);
                        }
                }
                current = current->next;
                index++;
        }
        analyzeBoardCube(mapDim, board);
        // printBoard(mapDim, board);

        tllmove moves = tll_init();
        char moveStr[INPUT_BUFFER_SIZE];
        strncpy(moveStr, (char*)ll->tail->data, INPUT_BUFFER_SIZE);
        parseMoves(&moves, moveStr);
        // printMoves(moves);

        position final = walkPath(mapDim, board, moves, startPos);
        int32 passwd = (1000 * final.y) + (4 * final.x) + final.dir;
        // printBoard(mapDim, board);

        printf("Part 2: Password: %d\n", passwd);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2022/Day22.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
                CubeSize = 4;
                CubeIndex = 1;
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

