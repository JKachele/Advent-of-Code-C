/*************************************************
 *File----------Day12.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Mar 16, 2026 17:10:32 UTC
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
// #include "../lib/tllist.h"
// #include "../util/vector.h"

#define INPUT_BUFFER_SIZE 4096

enum dir {
        NORTH,
        SOUTH,
        EAST,
        WEST,
        LEFT,
        RIGHT,
        FORWARD
};

typedef struct instruction {
        enum dir dir;
        int value;
} instr;

typedef struct ship {
        ivec2 pos;
        int facing;     // 0: North, 90: East, 180: South, 270: West
        ivec2 waypoint; // Relative to ship
} ship;

struct input {
        int numInstr;
        instr *instructions;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

ivec2 rotateWaypoint(ivec2 waypoint, int value, bool right) {
        // Convert value and direction to number of 90 degree rotations clockwise
        int rotations = 0;
        if (right)
                rotations = value / 90;
        else
                rotations = 4 - (value / 90);

        // Rotate 90 degrees clockwise by swaping x and y, then negate x
        for (int i = 0; i < rotations; i++) {
                ivec2 new = {{waypoint.y * -1, waypoint.x}};
                waypoint = new;
        }

        return waypoint;
}


ship updateShip(ship ship, enum dir dir, int value) {
        switch (dir) {
        case NORTH:
                ship.waypoint.y -= value;
                return ship;
        case SOUTH:
                ship.waypoint.y += value;
                return ship;
        case EAST:
                ship.waypoint.x += value;
                return ship;
        case WEST:
                ship.waypoint.x -= value;
                return ship;
        case LEFT:
                ship.waypoint = rotateWaypoint(ship.waypoint, value, false);
                return ship;
        case RIGHT:
                ship.waypoint = rotateWaypoint(ship.waypoint, value, true);
                return ship;
        case FORWARD:
                ship.pos.y += ship.waypoint.y * value;
                ship.pos.x += ship.waypoint.x * value;
                return ship;
        }

        // Unreachable
        return ship;
}

int updateFacing(int facing, int change, bool right) {
        // Add or subtract change for right or left turns
        if (right)
                facing += change;
        else
                facing -= change;

        // Make sure all values are 0-359
        while (facing < 0)
                facing += 360;
        facing %= 360;

        return facing;
}

ship moveForward(ship ship, int value) {
        switch (ship.facing) {
        case 0:
                ship.pos.y -= value;
                return ship;
        case 90:
                ship.pos.x += value;
                return ship;
        case 180:
                ship.pos.y += value;
                return ship;
        case 270:
                ship.pos.x -= value;
                return ship;
        default:
                printf("Error: Invalid direction for moveForward: %d\n", ship.facing);
                return ship;
        }
}

ship moveShip(ship ship, enum dir dir, int value) {
        switch (dir) {
        case NORTH:
                ship.pos.y -= value;
                return ship;
        case SOUTH:
                ship.pos.y += value;
                return ship;
        case EAST:
                ship.pos.x += value;
                return ship;
        case WEST:
                ship.pos.x -= value;
                return ship;
        case LEFT:
                ship.facing = updateFacing(ship.facing, value, false);
                return ship;
        case RIGHT:
                ship.facing = updateFacing(ship.facing, value, true);
                return ship;
        case FORWARD:
                return moveForward(ship, value);
        }

        // Unreachable
        return ship;
}

void part1(struct input input) {
        int numInstr = input.numInstr;
        instr *instructions = input.instructions;

        // Start at 0,0 facing East (Waypoint doesn't matter for Part 1)
        ship ship = {{{0, 0}}, 90, {{0, 0}}};
        for (int i = 0; i < numInstr; i++) {
                ship = moveShip(ship, instructions[i].dir, instructions[i].value);
        }

        // Get distance from start
        int distance = abs(ship.pos.x) + abs(ship.pos.y);

        printf("Part 1: %d\n\n", distance);
}

void part2(struct input input) {
        int numInstr = input.numInstr;
        instr *instructions = input.instructions;

        // Start at 0,0, Waypoint at 10,-1 (Facing doesn't matter for Part 2)
        ship ship = {{{0, 0}}, 0, {{10, -1}}};

        for (int i = 0; i < numInstr; i++) {
                ship = updateShip(ship, instructions[i].dir, instructions[i].value);
        }

        // Get distance from start
        int distance = abs(ship.pos.x) + abs(ship.pos.y);

        printf("Part 2: %d\n", distance);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        int numInstr = ll->length;
        input.numInstr = numInstr;
        input.instructions = malloc(numInstr * sizeof(instr));
        instr *instructions = input.instructions;

        int i = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                char dir = str[0];
                instructions[i].value = strtol(str+1, NULL, 10);

                switch (dir) {
                case 'N':
                        instructions[i].dir = NORTH;
                        break;
                case 'S':
                        instructions[i].dir = SOUTH;
                        break;
                case 'E':
                        instructions[i].dir = EAST;
                        break;
                case 'W':
                        instructions[i].dir = WEST;
                        break;
                case 'L':
                        instructions[i].dir = LEFT;
                        break;
                case 'R':
                        instructions[i].dir = RIGHT;
                        break;
                case 'F':
                        instructions[i].dir = FORWARD;
                        break;
                default:
                        printf("Invalid direction: %c\n", dir);
                }

                i++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2020/Day12.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2020/Day12.txt";
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

