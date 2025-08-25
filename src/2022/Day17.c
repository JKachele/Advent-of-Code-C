/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Aug 12, 2025 12:43:32 UTC
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "../lib/tllist.h"
#include "../util/util.h"

#define Width 7
#define Total 2022
#define NumRocks 5
#define MaxRockSize 5

/*
 * Positive y is up
 * Positive x is to the right
 * +
 * ^
 * |
 * |
 * Y X--->+
 */

typedef enum {
        LEFT,
        RIGHT
} dir;

typedef tll(uint8) tlluint8;

struct seen {
        int32 count;
        int64 pieceCount;
        int64 top;
};

struct state {
        int32 jetIndex;
        int32 numJets;
        int32 rockIndex;
        int64 numRocks;
        int64 top;
        tlluint8 chamber;
        lvec2 pos;
        // Hash table indexed by (jetIndex << 3) + rockIndex
        struct seen seen[100000];
        int64 addedBySeen;
};

struct rock {
        int8 numPieces;
        lvec2 size;
        lvec2 pieces[MaxRockSize];
};

// Each rock is stored as offsets to the bottom left corner of the shape
const struct rock Rocks[NumRocks] = {
        {   // H Line
                4,
                {4, 1},
                {
                        {0, 0},
                        {1, 0},
                        {2, 0},
                        {3, 0}
                }
        },{ // Plus
                5,
                {3, 3},
                {
                        {0, 1},
                        {1, 0},
                        {1, 1},
                        {1, 2},
                        {2, 1}
                }
        },{ // J or backwards L
                5,
                {3, 3},
                {
                        {0, 0},
                        {1, 0},
                        {2, 0},
                        {2, 1},
                        {2, 2}
                }
        },{ // V Line
                4,
                {1, 4},
                {
                        {0, 0},
                        {0, 1},
                        {0, 2},
                        {0, 3}
                }
        },{ // Square
                4,
                {2, 2},
                {
                        {0, 0},
                        {1, 0},
                        {0, 1},
                        {1, 1}
                }
        }
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void adjustRockCoords(struct rock *rock, lvec2 pos) {
        for (int i=0; i<rock->numPieces; i++) {
                rock->pieces[i].x += pos.x;
                rock->pieces[i].y += pos.y;
        }
}

void printChamber(struct state state, int32 numLines) {
        struct rock rock = Rocks[state.rockIndex];
        adjustRockCoords(&rock, state.pos);
        int32 rockTop = state.pos.y + rock.size.y - 1;

        // Print lines above chamber top
        int32 heightDiff = rockTop - state.top;
        for (int i=heightDiff; i>0; i--) {
                int32 y = state.top + i;
                char line[Width + 3] = "|.......|";
                for (int j=0; j<rock.numPieces; j++) {
                        if (rock.pieces[j].y == y)
                                line[rock.pieces[j].x + 1] = '@';
                }
                printf("%s\n", line);
        }

        // Print rest of chamber
        int32 lines = 0;
        tll_rforeach(state.chamber, it) {
                char line[Width + 3] = "|.......|";
                int y = state.top - lines;
                // If new rock is at this height, print it
                if (state.pos.y <= y) {
                        for (int j=0; j<rock.numPieces; j++) {
                                if (rock.pieces[j].y == y)
                                        line[rock.pieces[j].x + 1] = '@';
                        }
                }

                uint8 chamberLine = it->item;
                for (int i=0; i<Width; i++) {
                        int8 mask = 1<<i;
                        if (chamberLine & mask)
                                line[i + 1] = '#';
                }

                printf("%s\n", line);
                lines++;
                if (numLines > 0 && lines >= numLines) break;
        }

        printf("+-------+\n\n");
}       

bool isValidMove(struct state *state, lvec2 pos) {
        // Check for pos out of bounds
        if (pos.x < 0 || pos.x >= Width || pos.y < 0)
                return false;

        struct rock rock = Rocks[state->rockIndex];
        adjustRockCoords(&rock, pos);

        // Check for out of bounds
        for (int i=0; i<rock.numPieces; i++) {
                if (rock.pieces[i].x < 0 || rock.pieces[i].x >= Width)
                        return false;
                if (rock.pieces[i].y < 0)
                        return false;
        }

        // Check for collision with other rocks
        int64 y = state->top;
        int64 rockTop = state->pos.y + rock.size.y - 1;
        tll_rforeach(state->chamber, it) {
                if (y < pos.y) break;
                if (y > rockTop) {
                        y--;
                        continue;
                }
                uint8 line = it->item;
                for (int i=0; i<rock.numPieces; i++) {
                        if (rock.pieces[i].y != y) continue;
                        uint8 mask = 1 << rock.pieces[i].x;
                        if (line & mask)
                                return false;
                }
                y--;
        }

        return true;
}

void moveRock(struct state *state, dir jets[], bool print) {
        if(print)
                printChamber(*state, 10);
        while (true) {
                // Move rock to the side by jet
                lvec2 pos = state->pos;
                dir jet = jets[state->jetIndex];
                state->jetIndex++;
                if (state->jetIndex >= state->numJets)
                        state->jetIndex -= state->numJets;
                if (jet == LEFT) {
                        pos.x -= 1;
                } else {
                        pos.x += 1;
                }
                if (isValidMove(state, pos))
                        state->pos = pos;
                if (print) {
                        printf("%s\n", jet == LEFT? "Left" : "Right");
                        printChamber(*state, 10);
                }

                // Move rock down
                pos = state->pos;
                pos.y -= 1;
                if (isValidMove(state, pos))
                        state->pos = pos;
                else
                        break;
                if (print)
                        printChamber(*state, 10);
        }
        if (print)
                printChamber(*state, 10);
}

void setChamber(struct state *state) {
        struct rock rock = Rocks[state->rockIndex];
        lvec2 pos = state->pos;
        adjustRockCoords(&rock, pos);

        // Modify existing lines
        int64 y = state->top;
        tll_rforeach(state->chamber, it) {
                if (y < pos.y) break;
                for (int i=0; i<rock.numPieces; i++) {
                        if (rock.pieces[i].y != y) continue;
                        uint8 add = 1 << rock.pieces[i].x;
                        it->item = it->item | add;
                }
                y--;
        }

        // Add lines to chamber
        int64 rockTop = state->pos.y + rock.size.y - 1;
        for (int y=state->top+1; y<=rockTop; y++) {
                uint8 line = 0;
                for (int j=0; j<rock.numPieces; j++) {
                        if (rock.pieces[j].y == y) {
                                uint8 add = 1 << rock.pieces[j].x;
                                line = line | add;
                        }
                }
                tll_push_back(state->chamber, line);
        }
        state->top = tll_length(state->chamber) - 1;

        // So print function doesnt print this
        state->pos = (lvec2){-7, -7};
}

void addSeen(struct state *state, int64 target) {
        int32 hashIndex = (state->jetIndex << 3) + state->rockIndex;
        struct seen *seen = &(state->seen[hashIndex]);
        if (seen->count >= 2) {
                int64 dTop = state->top - seen->top;
                int64 dPieceCount = state->numRocks - seen->pieceCount;
                int64 numRepeats = (target - state->numRocks) / dPieceCount;
                state->addedBySeen += numRepeats * dTop;
                state->numRocks += numRepeats * dPieceCount;
        }
        seen->count++;
        seen->top = state->top;
        seen->pieceCount = state->numRocks;
}

int64 rockStack(int32 numJets, dir jets[], int64 target) {
        struct state state = {0};
        state.numJets = numJets;
        // NOTE: Numbers are backwards. LSB is the left wall, MSB is right wall
        state.chamber = (tlluint8)tll_init();

        state.top = -1;
        state.jetIndex = 0;
        state.rockIndex = 0;
        state.numRocks = 0;
        while (state.numRocks < target) {
                // Place new rock at x=2, y=top+4
                state.pos = (lvec2){2, state.top + 4};
                moveRock(&state, jets, false);
                setChamber(&state);
                addSeen(&state, target);

                state.numRocks++;
                state.rockIndex++;
                if (state.rockIndex >= NumRocks) state.rockIndex -= NumRocks;
        }

        // printChamber(state, -1);
        return state.top + state.addedBySeen + 1;
}

void part1(const char *input) {
        const int64 Target = 2022;

        int32 numJets = strlen(input);
        char str[numJets+1];
        strncpy(str, input, numJets+1);
        // printf("%s\n", str);
        // printf("Num Jets: %d\n", numJets);
        
        dir jets[numJets];
        for (int i=0; i<numJets; i++) {
                if (str[i] == '<')
                        jets[i] = LEFT;
                else if (str[i] == '>')
                        jets[i] = RIGHT;
                else
                        printf("Invalid jet direction: %c\n", str[i]);
        }

        int64 height = rockStack(numJets, jets, Target);

        printf("Part 1: Rock stack height: %ld\n", height);
}

void part2(const char *input) {
        const int64 Target = 1000000000000;

        int32 numJets = strlen(input);
        char str[numJets+1];
        strncpy(str, input, numJets+1);
        // printf("%s\n", str);
        // printf("Num Jets: %d\n", numJets);
        
        dir jets[numJets];
        for (int i=0; i<numJets; i++) {
                if (str[i] == '<')
                        jets[i] = LEFT;
                else if (str[i] == '>')
                        jets[i] = RIGHT;
                else
                        printf("Invalid jet direction: %c\n", str[i]);
        }

        int64 height = rockStack(numJets, jets, Target);

        printf("Part 2: Rock stack height: %ld\n", height);
}

void getInput(const char *fileName, int32 buffer, char *input) {
        FILE *inputFile = fopen(fileName, "r");     // Open file in read mode
        char *line = malloc(buffer);

        if (inputFile == NULL) {    // If file dosent exist, exit
                exit(EXIT_FAILURE);
        }

        // set "line" to the line in the file and test for end of file
        fgets(line, buffer, inputFile);
        line[strlen(line) - 1] = '\0';  // Remove newline character from string
        strncpy(input, line, buffer);

        fclose(inputFile);
        if (line) {
                free(line);
        }
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        const int32 Buffer = 0x3FFF;
        char input[Buffer];
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                getInput("assets/tests/2022/Day17.txt", Buffer, input);
                Debug = true;
        } else {
                getInput("assets/inputs/2022/Day17.txt", Buffer, input);
        }

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

