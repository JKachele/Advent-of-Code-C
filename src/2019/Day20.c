/*************************************************
 *File----------Day20.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Apr 13, 2026 18:26:04 UTC
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
#include "../lib/tllist.h"

#define INPUT_BUFFER_SIZE 16384

typedef enum celltype {
        OPEN,
        WALL,
        PORTAL
} celltype;

typedef enum direction {
        RIGHT,
        DOWN,
        LEFT,
        UP
} direction;

typedef tll(ivec3) tllivec3;
typedef tll(int) tllint;

typedef struct move {
        ivec3 pos;
        int dist;
} move;
typedef tll(move) tllmove;

typedef struct cell {
        celltype type;
        // Portal-specific fields
        u16 id;
        ivec2 dest;
        direction dir;
        bool outer;
        // Pathfinding-specific fields
        tllint visited;
} cell;

struct input {
        ivec2 size;
        cell **map;
        ivec2 start;
        ivec2 end;
};

const ivec2 DIRS[] = {{{1, 0}}, {{0, 1}}, {{-1, 0}}, {{0, -1}}};
const ivec3 DIRS3[] = {{{1, 0, 0}}, {{0, 1, 0}}, {{-1, 0, 0}}, {{0, -1, 0}}};
const u16 IN_ID = 0;    // "AA"
const u16 OUT_ID = 675; // "ZZ"

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printMap(ivec2 size, cell map[][size.x]) {
        if (!Debug) return;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (map[y][x].type == OPEN)
                                printf(" ");
                        if (map[y][x].type == WALL)
                                printf("\xE2\x96\x92");
                                // printf("#");
                        if (map[y][x].type == PORTAL && map[y][x].outer)
                                // printf("%c", map[y][x].id % 26 + 'A');
                                printf("O");
                        if (map[y][x].type == PORTAL && !map[y][x].outer)
                                // printf("%c", map[y][x].id % 26 + 'A');
                                printf("I");
                }
                printf("\n");
        }
        printf("\n");
}

u16 portalId(char c1, char c2) {
        u16 id = c1 - 'A';
        id *= 26;
        id += c2 - 'A';
        return id;
}

bool validPos(ivec2 pos, ivec2 size) {
        return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}

bool validPos2(ivec3 pos, ivec2 size) {
        return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y && pos.z >= 0;
}

bool intInList(tllint list, int i) {
        tll_foreach(list, it) {
                if (it->item == i)
                        return true;
        }
        return false;
}

ivec3 portalDest(cell cur, ivec3 pos, ivec3 newPos, bool recurse) {
        if (cur.id == IN_ID || cur.id == OUT_ID) {
                if (pos.z == 0 || !recurse)
                        return newPos;
                // In and Out cells in inner layers are walls. Return invalis pos to ignore
                return (ivec3){{-1, -1, -1}};
        }
        
        // Outer portals: Return 1 layer up, Inner portals: return 1 layer down
        if (cur.outer) {
                // Outer portals on outer layer are walls. Return invalid pos to ignore
                if (pos.z == 0 && recurse)
                        return (ivec3){{-1, -1, -1}};
                newPos = (ivec3){{cur.dest.x, cur.dest.y, pos.z - 1}};
        } else {
                newPos = (ivec3){{cur.dest.x, cur.dest.y, pos.z + 1}};
        }

        if (!recurse)
                newPos.z = 0;

        return newPos;
}

tllivec3 nextCells(ivec2 size, cell map[][size.x], ivec3 pos, bool recurse) {
        tllivec3 nextCells = tll_init();

        cell cur = map[pos.y][pos.x];
        for (int i = 0; i < 4; i++) {
                ivec3 newPos = ivec3Add(pos, DIRS3[i]);
                if (cur.type == PORTAL && (int)cur.dir == i) {
                        newPos = portalDest(cur, pos, newPos, recurse);
                }
                if (!validPos2(newPos, size))
                        continue;
                cell next = map[newPos.y][newPos.x];
                if (next.type != WALL && !intInList(next.visited, newPos.z))
                        tll_push_back(nextCells, newPos);
        }
        return nextCells;
}

void addMoves(tllmove *queue, move m) {
        // Add move to queue, sorting from least to most dist
        // If a move with the same pos is found,
        // if new move has been added, remove old move, else ignore new move and return
        bool added = false;
        tll_foreach(*queue, it) {
                move m2 = it->item;
                if (!added && m.dist < m2.dist) {
                        tll_insert_before(*queue, it, m);
                        added = true;
                }
                if (ivec3Eq(m.pos, m2.pos)) {
                        if (added) {
                                tll_remove(*queue, it);
                                return;
                        } else {
                                return;
                        }
                }
        }

        // If move has not been added, add it to the end
        if (!added)
                tll_push_back(*queue, m);
}

int dijkstra(ivec2 size, cell map[][size.x], ivec3 start, ivec3 end, bool recurse) {
        tllmove queue = tll_init();
        move init = {start, 0};
        tll_push_back(queue, init);

        while (tll_length(queue) > 0) {
                move curMove = tll_pop_front(queue);
                cell *cur = &map[curMove.pos.y][curMove.pos.x];
                tll_push_back(cur->visited, curMove.pos.z);

                if (ivec3Eq(curMove.pos, end)) {
                        return curMove.dist;
                }

                tllivec3 next = nextCells(size, map, curMove.pos, recurse);
                tll_foreach(next, it) {
                        move newMove = {it->item, curMove.dist + 1};
                        addMoves(&queue, newMove);
                }
        }

        return -1;
}

void part1(struct input *input) {
        ivec2 size = input->size;
        cell (*map)[size.x] = (cell(*)[size.x])input->map;
        printMap(size, map);

        ivec3 start = {{input->start.x, input->start.y, 0}};
        ivec3 end = {{input->end.x, input->end.y, 0}};
        int dist = dijkstra(size, map, start, end, false);

        printf("Part 1: %d\n\n", dist);
}

void part2(struct input *input) {
        ivec2 size = input->size;
        cell (*map)[size.x] = (cell(*)[size.x])input->map;

        // Clear visited lists from part 1
        for (int y = 0; y < size.y; y++)
                for (int x = 0; x < size.x; x++)
                        tll_free(map[y][x].visited);

        ivec3 start = {{input->start.x, input->start.y, 0}};
        ivec3 end = {{input->end.x, input->end.y, 0}};
        int dist = dijkstra(size, map, start, end, true);

        printf("Part 2: %d\n", dist);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        ivec2 size = {{getLongestLine(ll) - 4, ll->length - 4}}; // 2 char border for portal lables
        input.size = size;
        input.map = calloc(size.x * size.y, sizeof(cell));
        cell (*map)[size.x] = (cell(*)[size.x])input.map;

        // Convert linked lists to 2d array
        ivec2 size2 = {{size.x + 4, size.y + 4}}; // add 4 to account for portal labels
        char (*charMap)[size2.x] = (char(*)[size2.x])calloc(size2.x * size2.y, sizeof(char));
        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < size2.x; x++) {
                        charMap[y][x] = str[x];
                }
                y++;
        }

        // Get cell map, Ignore portals for now
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        char c = charMap[y + 2][x + 2];
                        if (c == '#') 
                                map[y][x].type = WALL;
                        else
                                map[y][x].type = OPEN;
                }
        }

        // Hashmap for portals
        ivec2 *portals = calloc(0x10000, sizeof(ivec2));

        // Link portals
        for (int y = 0; y < size2.y; y++) {
                for (int x = 0; x < size2.x; x++) {
                        char c = charMap[y][x];
                        if (c < 'A' || c > 'Z')
                                continue;

                        int dir = -1;
                        for (int i = 0; i < 4; i++) {
                                ivec2 new = ivec2Add((ivec2){{x, y}}, DIRS[i]);
                                if (!validPos(new, size2)) continue;
                                if (charMap[new.y][new.x] == '.') {
                                        dir = i;
                                        break;
                                }
                        }
                        // Only check if next to path. Prevents double checking a portal
                        if (dir == -1)
                                continue;

                        u16 id = 0;
                        ivec2 pos = {0};
                        if (dir == 0) {
                                id = portalId(charMap[y][x-1], c);
                                pos = (ivec2){{x-1, y-2}};
                        } else if (dir == 1) {
                                id = portalId(charMap[y-1][x], c);
                                pos = (ivec2){{x-2, y-1}};
                        } else if (dir == 2) {
                                id = portalId(c, charMap[y][x+1]);
                                pos = (ivec2){{x-3, y-2}};
                        } else if (dir == 3) {
                                id = portalId(c, charMap[y+1][x]);
                                pos = (ivec2){{x-2, y-3}};
                        }

                        if (id == IN_ID)
                                input.start = pos;
                        if (id == OUT_ID)
                                input.end = pos;

                        map[pos.y][pos.x].type = PORTAL;
                        map[pos.y][pos.x].id = id;
                        map[pos.y][pos.x].dir = (dir + 2) % 4;
                        if (pos.x == 0 || pos.y == 0 || pos.x == size.x - 1 || pos.y == size.y - 1)
                                map[pos.y][pos.x].outer = true;

                        if (portals[id].x == 0 && portals[id].y == 0) {
                                portals[id] = pos;
                        } else {
                                map[pos.y][pos.x].dest = portals[id];
                                map[portals[id].y][portals[id].x].dest = pos;
                        }
                }
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2019/Day20.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2019/Day20.txt";
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

