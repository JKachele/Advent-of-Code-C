/*************************************************
 *File----------Day9.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 02, 2025 17:56:44 EDT
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

typedef tll(ivec2) ivec2tll;

static const bool DEBUG = true;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (DEBUG)
                vprintf(format, args);
        va_end(args);
}

int max(int a, int b) {
        return a > b ? a : b;
}

void ivec2tllAdd(ivec2tll *list, ivec2 add) {
        tll_foreach(*list, it) {
                ivec2 pos = it->item;
                if (ivec2Eq(pos, add))
                        return;
        }
        tll_push_back(*list, add);
}

bool adjacent(ivec2 a, ivec2 b) {
        int diffX = abs(a.x - b.x);
        int diffY = abs(a.y - b.y);

        return (diffX < 2 && diffY < 2);
}

void moveRope(ivec2 *head, ivec2 *tail, ivec2tll *visited,
                direction dir, int moves) {
        ivec2 dirs[4] = {{0,-1},{1,0},{0,1},{-1,0}};

        // Move head of rope
        head->x += (dirs[dir].x * moves);
        head->y += (dirs[dir].y * moves);

        // If head and tail are still adjacent, tail doent move
        if (adjacent(*head, *tail)) goto debugExit;

        // Move tail
        ivec2 newTail;
        newTail.x = head->x - dirs[dir].x;
        newTail.y = head->y - dirs[dir].y;

        int dist = max(abs(newTail.x - tail->x), abs(newTail.y - tail->y));

        for (int i = 0; i < dist; i++) {
                ivec2 visit;
                visit.x = newTail.x - (dirs[dir].x * i);
                visit.y = newTail.y - (dirs[dir].y * i);
                ivec2tllAdd(visited, visit);
        }

        *tail = newTail;
debugExit:
        debugP("Head: (%d, %d), Tail: (%d, %d)\n",
                        head->x, head->y, tail->x, tail->y);
}

void part1(llist *ll) {
        ivec2 head = {.x = 0, .y = 0};
        ivec2 tail = {.x = 0, .y = 0};
        ivec2tll visited = tll_init();
        tll_push_back(visited, tail);

        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);

                direction dir;
                switch (str[0]) {
                case 'U':
                        dir = NORTH;
                        break;
                case 'R':
                        dir = EAST;
                        break;
                case 'D':
                        dir = SOUTH;
                        break;
                case 'L':
                        dir = WEST;
                        break;
                default:
                        printf("UNKNOWN DIRECTION CHAR '%c'\n", str[0]);
                }

                int moves = strtol(str + 2, (char**)NULL, 10);

                debugP("%s   ", str);
                moveRope(&head, &tail, &visited, dir, moves);

                current = current->next;
        }

        int numVisited = tll_length(visited);
        
        printf("Part 1: Num visited: %d\n\n", numVisited);
}

void part2(llist *ll) {
        llNode *current = ll->head;
        while(current != NULL) {
                char str[BUFFER_SIZE];
                strncpy(str, (char*)current->data, BUFFER_SIZE);
                current = current->next;
        }
        printf("Part 2: \n");
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day9.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day9.txt");
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

