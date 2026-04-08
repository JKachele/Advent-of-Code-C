/*************************************************
 *File----------Day17.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 08, 2026 14:27:48 UTC
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

typedef enum direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
} direction;

typedef struct move {
        int function;
        bool right;
        int steps;
} move;
typedef tal(move) talmove;

typedef struct robotinstr {
        talmove routine;
        talmove functs[3];
} robotinstr;

typedef struct robot {
        ivec2 pos;
        direction dir;
} robot;

struct input {
        talint64 intcode;
        ivec2 size;
        bool **map;
        robot rob;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, bool map[][size.x]) {
        if (!Debug) return;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        printf("%c", map[y][x] ? '#' : '.');
                }
                printf("\n");
        }
}

void printMoves(talmove moves) {
        if (!Debug) return;
        for (int i = 0; i < (int)moves.length; i++) {
                move m = moves.array[i];
                if (m.function == -1)
                        printf("%c%d ", m.right ? 'R' : 'L', m.steps);
                else
                        printf("*%c* ", m.function + 'A');
        }
        printf("\n");
}

void printInstructions(robotinstr instr) {
        if (!Debug) return;
        printf("A: ");
        printMoves(instr.functs[0]);
        printf("B: ");
        printMoves(instr.functs[1]);
        printf("C: ");
        printMoves(instr.functs[2]);
        printMoves(instr.routine);
        printf("\n");
}

void robotInstrDestroy(robotinstr *instr) {
        tal_destroy(instr->routine);
        tal_destroy(instr->functs[0]);
        tal_destroy(instr->functs[1]);
        tal_destroy(instr->functs[2]);
}

int64 findIntersections(ivec2 size, bool map[][size.x]) {
        const ivec2 dirVecs[] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};

        int64 intSum = 0;
        for (int y = 1; y < size.y - 1; y++) {
                for (int x = 1; x < size.x - 1; x++) {
                        if (!map[y][x]) continue;

                        int numAdjacent = 0;
                        for (int d = 0; d < 4; d++) {
                                if (map[y + dirVecs[d].y][x + dirVecs[d].x])
                                        numAdjacent++;
                        }
                        if (numAdjacent == 4)
                                intSum += x * y;
                }
        }

        return intSum;
}

bool validPos(ivec2 size, ivec2 pos) {
        bool valid = pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
        return valid;
}

robotinstr copyRobotInstr(robotinstr instr) {
        robotinstr copy = {0};
        for (int i = 0; i < (int)instr.routine.length; i++)
                tal_add(copy.routine, instr.routine.array[i]);

        for (int i = 0; i < 3; i++)
                for (int j = 0; j < (int)instr.functs[i].length; j++)
                        tal_add(copy.functs[i], instr.functs[i].array[j]);

        return copy;
}

void copyRobotInstrPtr(robotinstr *copy, robotinstr instr) {
        tal_destroy(copy->routine);
        for (int i = 0; i < (int)instr.routine.length; i++)
                tal_add(copy->routine, instr.routine.array[i]);

        for (int i = 0; i < 3; i++) {
                tal_destroy(copy->functs[i]);
                for (int j = 0; j < (int)instr.functs[i].length; j++)
                        tal_add(copy->functs[i], instr.functs[i].array[j]);
        }
}

talmove getRobotMoves(ivec2 size, bool map[][size.x], robot rob) {
        const ivec2 dirVecs[] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};

        talmove moves = tal_init();
        for (;;) {
                move curMove = {0};
                curMove.function = -1;

                // Check right and left for a scaffold to move to
                ivec2 right = ivec2Add(rob.pos, dirVecs[(rob.dir + 1) % 4]);
                ivec2 left = ivec2Add(rob.pos, dirVecs[(rob.dir + 3) % 4]);

                ivec2 moveDir;
                if (validPos(size, right) && map[right.y][right.x]) {
                        curMove.right = true;
                        rob.dir = (rob.dir + 1) % 4;
                        moveDir = dirVecs[rob.dir];
                } else if (validPos(size, left) && map[left.y][left.x]) {
                        curMove.right = false;
                        rob.dir = (rob.dir + 3) % 4;
                        moveDir = dirVecs[rob.dir];
                } else {
                        // If nowhere to move, Robot has reached end
                        break;
                }

                // Move in direction until we can't
                ivec2 nextPos = ivec2Add(rob.pos, moveDir);
                while (validPos(size, nextPos) && map[nextPos.y][nextPos.x]) {
                        curMove.steps++;
                        rob.pos = nextPos;
                        nextPos = ivec2Add(rob.pos, moveDir);
                }
                tal_add(moves, curMove);
        }

        return moves;
}

void replaceFunction(robotinstr *instr, int function) {
        talmove funct = instr->functs[function];

        int i = 0;
        int found = -1;
        while (i < (int)instr->routine.length) {
                move m = instr->routine.array[i];
                if (found == -1) {
                        if (m.function == -1 && m.right == funct.array[0].right &&
                                        m.steps == funct.array[0].steps) {
                                found = i;
                        }
                        i++;
                        continue;
                }

                int functIndex = i - found;
                move f = funct.array[functIndex];
                if (m.function == -1 && m.right == f.right && m.steps == f.steps) {
                        i++;
                        functIndex++;
                } else {
                        i = found + 1;
                        found = -1;
                        continue;
                }

                // If full function was found, remove moves from routine and add function
                if (functIndex == (int)funct.length) {
                        for (int j = 0; j < (int)funct.length; j++)
                                tal_remove(instr->routine, found);
                        move fun = {function, 0, 0};
                        tal_insert(instr->routine, found, fun);
                        i = found + 1;
                        found = -1;
                        continue;
                }
        }
}

bool getRobotInstructions(robotinstr *instr, int function) {
        if (function > 2) {
                // If any non-function moves are left, return false and try again
                for (int i = 0; i < (int)instr->routine.length; i++) {
                        move m = instr->routine.array[i];
                        if (m.function == -1)
                                return false;
                }
                return true;
        }

        // Get longest function. Limit is 20 chars or until hit another movement function
        int chars = -1; // Start at -1 to account for no leading comma
        int i = 0;
        while (i < (int)instr->routine.length) {
                move m = instr->routine.array[i];
                if (m.function != -1) {
                        if (chars == -1) {
                                i++;
                                continue;
                        } else {
                                break;
                        }
                }

                // Comma, Direction, and 1 or 2 depending if steps is 1 or 2 digits
                chars += 2 + (m.steps < 10 ? 1 : 2);
                if (chars > 20)
                        break;

                // Remove move from routine and add to function
                tal_remove(instr->routine, i);
                tal_add(instr->functs[function], m);
        }
        move m = (move){function, 0, 0};
        tal_insert(instr->routine, i, m);

        replaceFunction(instr, function);

        // printf("%c: ", function + 'A');
        // printMoves(instr->functs[function]);
        // printMoves(instr->routine);
        // printf("\n");

        robotinstr copy = copyRobotInstr(*instr);
        while (!getRobotInstructions(&copy, function + 1)) {
                // The function can't be reduced past 2 moves
                if (instr->functs[function].length <= 2)
                        return false;

                // Remove last move of the function and try again
                move m = tal_pop_back(instr->functs[function]);
                for (int i = 0; i < (int)instr->routine.length; i++) {
                        move m2 = instr->routine.array[i];
                        if (m2.function == function) {
                                if (i + 1 < (int)instr->routine.length) {
                                        tal_insert(instr->routine, i+1, m);
                                } else {
                                        tal_add(instr->routine, m2);
                                        break;
                                }
                        }
                }
                replaceFunction(instr, function);

                // printf("%c: ", function + 'A');
                // printMoves(instr->functs[function]);
                // printMoves(instr->routine);
                // printf("\n");

                robotInstrDestroy(&copy);
                copy = copyRobotInstr(*instr);
        }
        copyRobotInstrPtr(instr, copy);
        robotInstrDestroy(&copy);

        return true;
}

void setMovementCode(talint64 *inputs, robotinstr instr) {
        // Main movement routine
        for (int i = 0; i < (int)instr.routine.length; i++) {
                move m = instr.routine.array[i];
                tal_add(*inputs, m.function + 'A');
                tal_add(*inputs, ',');
        }
        // Replace last comma with newline
        inputs->array[inputs->length - 1] = '\n';

        // Movement functions
        for (int i = 0; i < 3; i++) {
                for (int j = 0; j < (int)instr.functs[i].length; j++) {
                        move m = instr.functs[i].array[j];
                        char dir = m.right ? 'R' : 'L';
                        tal_add(*inputs, dir);
                        tal_add(*inputs, ',');
                        if (m.steps < 10) {
                                tal_add(*inputs, m.steps + '0');
                        } else {
                                tal_add(*inputs, (m.steps / 10) + '0');
                                tal_add(*inputs, (m.steps % 10) + '0');
                        }
                        tal_add(*inputs, ',');
                }
                inputs->array[inputs->length - 1] = '\n';
        }

        // Video feed
        tal_add(*inputs, 'n');
        tal_add(*inputs, '\n');
}

void part1(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);

        talint64 inputs = tal_init();
        talint64 outputs = tal_init();
        runIntcode(&intcode, (haltmode){0}, &inputs, &outputs);

        ivec2 size = {0};
        for (int i = 0; i < (int)outputs.length; i++) {
                char c = (char)outputs.array[i];
                debugp("%c", c);
                if (c == '\n') {
                        if (size.x == 0)
                                size.x = i;
                        size.y++;
                }
        }
        size.y--; // Output has an extra newline at the end

        bool (*map)[size.x] = (bool(*)[size.x])malloc(size.x * size.y * sizeof(bool));
        robot rob = {0}; // Robot named rob lol
        int i = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        char c = outputs.array[i++];
                        map[y][x] = (c != '.');
                        if (c == '^')
                                rob = (robot){{{x, y}}, UP};
                        else if (c == '>')
                                rob = (robot){{{x, y}}, RIGHT};
                        else if (c == 'v')
                                rob = (robot){{{x, y}}, DOWN};
                        else if (c == '<')
                                rob = (robot){{{x, y}}, LEFT};
                }
                i++;
        }

        input->size = size;
        input->map = (bool**)map;
        input->rob = rob;

        int64 intersections = findIntersections(size, map);

        printf("Part 1: %ld\n\n", intersections);
}

void part2(struct input *input) {
        talint64 intcode = copyIntcode(input->intcode);
        ivec2 size = input->size;
        bool (*map)[size.x] = (bool(*)[size.x])input->map;
        robot rob = input->rob;

        // Get moves to walk the robot
        talmove moves = getRobotMoves(size, map, rob);
        printMoves(moves);

        robotinstr instr = {0};
        instr.routine = moves;
        getRobotInstructions(&instr, 0);
        printInstructions(instr);

        // Update intcode to wake up vacuum
        intcode.array[0] = 2;

        // Setup input with movement code;
        talint64 inputs = tal_init();
        talint64 outputs = tal_init();
        setMovementCode(&inputs, instr);

        runIntcode(&intcode, (haltmode){0}, &inputs, &outputs);

        int64 dust = outputs.array[outputs.length - 1];

        printf("Part 2: %ld\n", dust);
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
                char *file = "assets/tests/2019/Day17.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day17.txt";
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

