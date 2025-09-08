/*************************************************
 *File----------Day23.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 08, 2025 16:21:21 UTC
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

typedef enum tileType {
        FOREST,
        PATH,
        SLOPE
} tiletype;

typedef enum direction {
        RIGHT,
        DOWN,
        LEFT,
        UP
} direction;

typedef struct tile {
        tiletype type;
        direction dir;
        bool vertex;
} tile;

typedef struct edge {
        int x;
        int y;
        int dist;
        direction dir;
} edge;

typedef struct vertex {
        int x;
        int y;
        int index;
        int adj[4];
} vertex;
typedef tll(vertex) tllvert;

typedef struct state {
        ivec2 pos;
        ivec2 prev;
        int32 cost;
} state;
typedef tll(state) tllstate;
typedef tll(ivec2) tllivec2;

typedef struct state2 {
        int vert;
        int dist;
        bool *seen;
} state2;
typedef tll(state2) tllstate2;

struct input {
        ivec2 size;
        tile **map;
        ivec2 start;
        ivec2 end;
};

static bool Debug = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, tile map[size.y][size.x]) {
        for (int y=0; y<size.y; y++) {
                for (int x=0; x<size.x; x++) {
                        if (map[y][x].type == FOREST) {
                                printf("#");
                                continue;
                        } else if (map[y][x].type == PATH) {
                                printf(".");
                                continue;
                        }
                        switch (map[y][x].dir) {
                        case RIGHT:
                                printf(">");
                                break;
                        case DOWN:
                                printf("v");
                                break;
                        case LEFT:
                                printf("<");
                                break;
                        case UP:
                                printf("^");
                                break;
                        }
                }
                printf("\n");
        }
        printf("\n");
}

void printAdjMat(int numV, int adjMat[numV][numV]) {
        printf("   ");
        for (int i=0; i<numV; i++) {
                printf("%d  ", i);
        }
        printf("\n");
        for (int i=0; i<numV; i++) {
                printf("%d: ", i);
                for (int j=0; j<numV; j++) {
                        if (adjMat[i][j] > 0)
                                printf("%02d ", adjMat[i][j]);
                        else
                                printf("   ");
                }
                printf("\n");
        }
        printf("\n");
}

// Add a state to the queue, sorting from highest cost to lowest cost
void addState(tllstate *queue, ivec2 pos, ivec2 prev, int32 cost) {
        bool added = false;
        tll_foreach(*queue, it) {
                state *s = &it->item;
                if (ivec2Eq(s->pos, pos)) {
                        if (cost > s->cost) {
                                s->cost = cost;
                                s->prev = prev;
                        }
                        added = true;
                        break;
                }
        }
        if (!added) {
                state new = {pos, prev, cost};
                tll_push_back(*queue, new);
        }
}

tllivec2 getNextPos(ivec2 size, tile map[][size.x], ivec2 pos, ivec2 prev) {
        ivec2 dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
        tllivec2 nextPoss = tll_init();

        tile curT = map[pos.y][pos.x];

        if (curT.type == SLOPE) {
                ivec2 next = addIVec2(pos, dirs[curT.dir]);
                if (ivec2Eq(next, prev))
                        return nextPoss;
                tll_push_back(nextPoss, next);
                return nextPoss;
        }

        for (int i=0; i<4; i++) {
                ivec2 nextP = addIVec2(pos, dirs[i]);
                if (ivec2Eq(nextP, prev))
                        continue;
                tile nextT = map[nextP.y][nextP.x];
                if (nextT.type != FOREST)
                        tll_push_back(nextPoss, nextP);
        }

        return nextPoss;
}

int32 bfs(ivec2 size, tile map[size.y][size.x], ivec2 start, ivec2 end) {
        tllstate queue = tll_init();
        state init = {start, start, 0};
        tll_push_back(queue, init);

        int32 longest = 0;
        while (tll_length(queue) > 0) {
                state curS = tll_pop_front(queue);
                ivec2 curP = curS.pos;
                // printf("Pos: %d,%d Cost: %d\n", curP.x, curP.y, curS.cost);

                if (ivec2Eq(curP, end)) {
                        if (curS.cost > longest)
                                longest = curS.cost;
                        // printf("Path found with cost %d\n", curS.cost);
                        continue;
                }

                // map[curP.y][curP.x].seen = true;
                tllivec2 nextPoss = getNextPos(size, map, curP, curS.prev);
                tll_foreach(nextPoss, it) {
                        addState(&queue, it->item, curP, curS.cost + 1);
                }
        }

        return longest;
}

tllvert getVertices(ivec2 size, tile map[][size.x], ivec2 start, ivec2 end) {
        ivec2 dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};

        tllvert verts = tll_init();
        int index = 0;
        for (int y=1; y<size.y-1; y++) {
                for (int x=1; x<size.x-1; x++) {
                        if (map[y][x].type == FOREST)
                                continue;
                        int numDirs = 0;
                        for (int i=0; i<4; i++) {
                                int x2 = x + dirs[i].x;
                                int y2 = y + dirs[i].y;
                                if (map[y2][x2].type != FOREST)
                                        numDirs++;
                        }
                        ivec2 pos = {{x, y}};
                        if (numDirs > 2 || ivec2Eq(pos, start) ||
                                        ivec2Eq(pos, end)) {
                                map[y][x].vertex = true;
                                vertex v = {0};
                                v.x = x;
                                v.y = y;
                                v.adj[0] = -1;
                                v.adj[1] = -1;
                                v.adj[2] = -1;
                                v.adj[3] = -1;
                                v.index = index++;
                                tll_push_back(verts, v);
                        }
                }
        }
        return verts;
}

edge walkEdge(ivec2 size, tile map[][size.x], vertex start, direction dir) {
        ivec2 dirs[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};

        ivec2 prev = {{start.x, start.y}};
        ivec2 cur = ivec2Add(prev, dirs[dir]);
        direction prevDir = dir;
        if (map[cur.y][cur.x].type == FOREST)
                return (edge){-1, -1, 0, 0};

        int dist = 0;
        while (true) {
                dist++;
                if (map[cur.y][cur.x].vertex)
                        return (edge){cur.x, cur.y, dist, prevDir};
                for (int i=0; i<4; i++) {
                        ivec2 nextP = ivec2Add(cur, dirs[i]);
                        if (ivec2Eq(nextP, prev))
                                continue;
                        tile nextT = map[nextP.y][nextP.x];
                        if (nextT.type != FOREST) {
                                prev = cur;
                                cur = nextP;
                                prevDir = i;
                                break;
                        }
                }
        }
}

void createGraph(ivec2 size, tile map[size.y][size.x], ivec2 start,
                 int32 numV, int adjMat[numV][numV], tllvert vertstll) {
        // Initialize adjacency matrix
        for (int y=0; y<numV; y++) {
                for (int x=0; x<numV; x++) {
                        adjMat[y][x] = 0;
                }
        }

        vertex verts[numV];
        int postoindex[1 << 16] = {0};
        tll_foreach(vertstll, it) {
                vertex v = it->item;
                verts[v.index] = v;
                int32 posid = (v.y << 8) | v.x;
                postoindex[posid] = v.index;
        }

        for (int i=0; i<numV; i++) {
                vertex *v = &verts[i];
                // printf("%d: (%d, %d)\n", v->index, v->x, v->y);
                for (int j=0; j<4; j++) {
                        int adj = v->adj[j];
                        if (adj != -1 && adjMat[i][adj] != 0)
                                continue;
                        edge e = walkEdge(size, map, *v, (direction)j);
                        if (e.x == -1)
                                continue;
                        // printf("\t(%d, %d): %d, %d\n", e.x, e.y, e.dist, e.dir);

                        int id = (e.y << 8) | e.x;
                        int eIndex = postoindex[id];
                        int oppDir = (e.dir + 2) % 4;
                        v->adj[j] = eIndex;
                        verts[eIndex].adj[oppDir] = i;
                        adjMat[i][eIndex] = e.dist;
                        adjMat[eIndex][i] = e.dist;
                }
        }

        return;
}

int32 getLongestPath(int numV, int adjMat[][numV]) {
        tllstate2 queue = tll_init();
        state2 init = {0};
        init.seen = calloc(numV, sizeof(bool));
        tll_push_back(queue, init);

        int longest = 0;
        while (tll_length(queue) > 0) {
                state2 curS = tll_pop_front(queue);
                int curI = curS.vert;

                if (curI == numV-1) {
                        if (curS.dist > longest)
                                longest = curS.dist;
                        continue;
                }

                for (int i=0; i<numV; i++) {
                        if (adjMat[curI][i] == 0 || curS.seen[i])
                                continue;
                        state2 new = {i, curS.dist + adjMat[curI][i], NULL};
                        new.seen = calloc(numV, sizeof(bool));
                        memcpy(new.seen, curS.seen, numV*sizeof(bool));
                        new.seen[curI] = true;
                        tll_push_back(queue, new);
                }
                free(curS.seen);
        }

        return longest;
}

void part1(struct input input) {
        ivec2 size = input.size;
        tile (*map)[size.x] = (tile(*)[size.x])input.map;
        // printMap(size, map);

        int32 longestPath = bfs(size, map, input.start, input.end);

        printf("Part 1: %d\n\n", longestPath);
}

void part2(struct input input) {
        ivec2 size = input.size;
        tile (*map)[size.x] = (tile(*)[size.x])input.map;
        // printMap(size, map);

        tllvert verts = getVertices(size, map, input.start, input.end);
        int32 numV = tll_length(verts);
        int adjMat[numV][numV];
        createGraph(size, map, input.start, numV, adjMat, verts);
        // printAdjMat(numV, adjMat);

        int32 longestPath = getLongestPath(numV, adjMat);

        printf("Part 2: %d\n\n", longestPath);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        // Adding extra line of forest at top and bottom
        ivec2 size = {{getLongestLine(ll), ll->length+2}};
        input.size = size;
        input.start.y = 1;
        tile **mapPtr = calloc(size.y * size.x, sizeof(tile));
        tile (*map)[size.x] = (tile(*)[size.x])mapPtr;

        llNode *current = ll->head;
        int y = 1;
        while(current != NULL) {
                char str[INPUT_BUFFER_SIZE];
                strncpy(str, (char*)current->data, INPUT_BUFFER_SIZE);

                for (int x=0; x<size.x; x++) {
                        switch (str[x]) {
                        case '#':
                                map[y][x].type = FOREST;
                                break;
                        case '.':
                                map[y][x].type = PATH;
                                if (y == 1) input.start.x = x;
                                input.end.x = x;
                                input.end.y = y;
                                break;
                        case '>':
                                map[y][x].type = SLOPE;
                                map[y][x].dir = RIGHT;
                                break;
                        case 'v':
                                map[y][x].type = SLOPE;
                                map[y][x].dir = DOWN;
                                break;
                        case '<':
                                map[y][x].type = SLOPE;
                                map[y][x].dir = LEFT;
                                break;
                        case '^':
                                map[y][x].type = SLOPE;
                                map[y][x].dir = UP;
                                break;
                        default:
                                printf("UNKNOWN CHAR: %c\n", str[x]);
                        }
                }

                current = current->next;
                y++;
        }
        // printMap(size, map);
        input.map = (tile**)map;

        return input;
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2023/Day23.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2023/Day23.txt";
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

