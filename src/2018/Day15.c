/*************************************************
 *File----------Day15.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Sep 22, 2026 09:07:28 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"
#include "../util/talist.h"
#include "../util/vector.h"
#include "../util/quicksort.h"
#include "../lib/tllist.h"

#define INPUT_BUFFER_SIZE 1024

typedef tll(ivec2) tllivec2;

typedef enum {
        OPEN,
        WALL,
        ELF,
        GOBLIN
} CellType;

struct cell {
        CellType type;
        int16 hp;
        bool visited;
        int8 elfDist;
        int8 goblinDist;
};

struct queueEntry {
        ivec2 pos;
        int8 dist;
};
typedef tll(struct queueEntry) tllqueue;

struct input {
        ivec2 size;
        struct cell **grid;
};

const ivec2 Dirs[4] = {{{0, -1}}, {{-1, 0}}, {{1, 0}}, {{0, 1}}};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printGrid(ivec2 size, struct cell grid[][size.x]) {
        for (int y = 0; y < size.y; y++) {
                tllivec2 units = tll_init();
                for (int x = 0; x < size.x; x++) {
                        ivec2 pos = {{x, y}};
                        if (grid[y][x].type == WALL) {
                                printf("#");
                        } else if (grid[y][x].type == ELF) {
                                tll_push_back(units, pos);
                                printf("E");
                        } else if (grid[y][x].type == GOBLIN) {
                                tll_push_back(units, pos);
                                printf("G");
                        } else {
                                printf(".");
                        }
                }
                tll_foreach(units, it) {
                        ivec2 unit = it->item;
                        printf(" %c(", grid[unit.y][unit.x].type == ELF ? 'E' : 'G');
                        printf("%d)", grid[unit.y][unit.x].hp);
                }
                printf("\n");
                tll_free(units);
        }
        printf("\n");
}

void copyGrid(ivec2 size, struct cell grid[][size.x], struct cell copy[][size.x]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        copy[y][x].type = grid[y][x].type;
                        copy[y][x].hp = grid[y][x].hp;
                        copy[y][x].visited = grid[y][x].visited;
                        copy[y][x].elfDist = grid[y][x].elfDist;
                        copy[y][x].goblinDist = grid[y][x].goblinDist;
                }
        }
}

int numUnits(ivec2 size, struct cell grid[][size.x], CellType type) {
        int num = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x].type == type)
                                num++;
                }
        }
        return num;
}

bool isInRange(ivec2 size, struct cell grid[][size.x], ivec2 pos, CellType enemy) {
        for (int i = 0; i < 4; i++) {
                ivec2 targetPos = ivec2Add(pos, Dirs[i]);
                struct cell target = grid[targetPos.y][targetPos.x];
                if (target.type == enemy) {
                        return true;
                }
        }
        return false;
}

void bfs(ivec2 size, struct cell grid[][size.x], ivec2 start, bool elf) {
        // Reset visited flag
        for (int y = 0; y < size.y; y++)
                for (int x = 0; x < size.x; x++)
                        grid[y][x].visited = false;

        tllqueue queue = tll_init();
        struct queueEntry initial = {start, 0};
        tll_push_back(queue, initial);

        while (tll_length(queue) > 0) {
                struct queueEntry cur = tll_pop_front(queue);
                struct cell *curCell = &grid[cur.pos.y][cur.pos.x];

                if (elf && curCell->elfDist > cur.dist)
                        curCell->elfDist = cur.dist;
                if (!elf && curCell->goblinDist > cur.dist)
                        curCell->goblinDist = cur.dist;

                for (int i = 0; i < 4; i++) {
                        ivec2 targetPos = ivec2Add(cur.pos, Dirs[i]);
                        struct cell *target = &grid[targetPos.y][targetPos.x];
                        if (target->type == OPEN && !target->visited) {
                                struct queueEntry next = {targetPos, cur.dist + 1};
                                tll_push_back(queue, next);
                                target->visited = true;
                        }
                }
        }
        tll_free(queue);
}

void setDists(ivec2 size, struct cell grid[][size.x]) {
        // Reset dists
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        grid[y][x].elfDist = INT8_MAX;
                        grid[y][x].goblinDist = INT8_MAX;
                }
        }

        // Find dist to nearest unit of type
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        ivec2 pos = {{x, y}};
                        if (grid[y][x].type == ELF)
                                bfs(size, grid, pos, true);
                        else if (grid[y][x].type == GOBLIN)
                                bfs(size, grid, pos, false);
                }
        }
}

ivec2 moveUnit(ivec2 size, struct cell grid[][size.x], ivec2 curPos) {
        struct cell *cur = &grid[curPos.y][curPos.x];
        CellType enemyType = cur->type == ELF ? GOBLIN : ELF;

        // If current is already in range, no move
        if (isInRange(size, grid, curPos, enemyType))
                return curPos;

        // Select cell to move to
        ivec2 movePos = {{-1, -1}};
        int8 minDist = INT8_MAX;
        for (int i = 0; i < 4; i++) {
                ivec2 targetPos = ivec2Add(curPos, Dirs[i]);
                struct cell target = grid[targetPos.y][targetPos.x];
                if (target.type != OPEN) continue;
                int8 dist = enemyType == ELF ? target.elfDist : target.goblinDist;
                if (dist < minDist) {
                        minDist = dist;
                        movePos = targetPos;
                }
        }

        // If no valid cells to move to, no move
        if (minDist == INT8_MAX)
                return curPos;

        // Move cell
        struct cell *new = &grid[movePos.y][movePos.x];
        new->type = cur->type;
        new->hp = cur->hp;
        cur->type = OPEN;
        cur->hp = 0;

        // Since a unit was moved, we need to recalculate the dists for that type
        setDists(size, grid);

        return movePos;
}

bool fightUnit(ivec2 size, struct cell grid[][size.x], ivec2 enemyPos, int power) {
        struct cell *enemy = &grid[enemyPos.y][enemyPos.x];
        enemy->hp -= power;
        if (enemy->hp <= 0) {
                enemy->type = OPEN;
                enemy->hp = 0;
                return true;
        }
        return false;
}

// If a unit was killed, return killed unit's pos
// If no enemy units left, return {-2, -2}, else return {-1, -1}
ivec2 unitTurn(ivec2 size, struct cell grid[][size.x], ivec2 curPos, int elfPower) {
        struct cell cur = grid[curPos.y][curPos.x];
        CellType enemyType = cur.type == ELF ? GOBLIN : ELF;

        if (numUnits(size, grid, enemyType) == 0)
                return (ivec2){{-2, -2}};

        curPos = moveUnit(size, grid, curPos);

        ivec2 enemyPos = {{-1, -1}};
        int16 minHP = UINT8_MAX;
        for (int i = 0; i < 4; i++) {
                ivec2 targetPos = ivec2Add(curPos, Dirs[i]);
                struct cell target = grid[targetPos.y][targetPos.x];
                if (target.type == enemyType && target.hp < minHP) {
                        minHP = target.hp;
                        enemyPos = targetPos;
                }
        }

        if (enemyPos.x == -1) return enemyPos;

        int power = cur.type == ELF ? elfPower : 3;
        if (fightUnit(size, grid, enemyPos, power)) {
                // Killed unit
                setDists(size, grid);
                return enemyPos;
        }

        return (ivec2){{-1, -1}};
}

tllivec2 getUnits(ivec2 size, struct cell grid[][size.x]) {
        tllivec2 units = tll_init();
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x].type == ELF || grid[y][x].type == GOBLIN) {
                                ivec2 pos = {{x, y}};
                                tll_push_back(units, pos);
                        }
                }
        }
        return units;
}

bool completeRound(ivec2 size, struct cell grid[][size.x], int elfPower) {
        tllivec2 units = getUnits(size, grid);
        tllivec2 killedUnits = tll_init();

        tll_foreach(units, it) {
                ivec2 unit = it->item;
                if (tll_length(killedUnits) > 0) {
                        bool found = false;
                        tll_foreach(killedUnits, itt) {
                                ivec2 killed = itt->item;
                                if (ivec2Eq(unit, killed)) {
                                        found = true;
                                        break;
                                }
                        }
                        if (found) continue;
                }

                ivec2 killed = unitTurn(size, grid, unit, elfPower);
                if (killed.x == -2) {
                        tll_free(units);
                        return false;
                }
                if (killed.x >= 0) {
                        tll_push_back(killedUnits, killed);
                }
        }
        tll_free(units);
        tll_free(killedUnits);
        return true;
}

void part1(struct input *input) {
        ivec2 size = input->size;
        struct cell (*grid)[size.x] =
                (struct cell(*)[size.x])calloc(size.x * size.y, sizeof(struct cell));
        copyGrid(size, (struct cell(*)[size.x])input->grid, grid);
        // printGrid(size, grid);

        // Set initial elf and goblin dists
        setDists(size, grid);

        // Complete rounds until no enemies are left
        int rounds = 0;
        for (;;) {
                if(!completeRound(size, grid, 3))
                        break;
                rounds++;
        }

        // Get battle outcome
        int hpRemaining = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (grid[y][x].type == ELF || grid[y][x].type == GOBLIN)
                                hpRemaining += grid[y][x].hp;
                }
        }

        int outcome = hpRemaining * rounds;

        printf("Part 1: Battle outcome = %d\n\n", outcome);
        free(grid);
}

void part2(struct input *input) {
        ivec2 size = input->size;
        struct cell (*grid)[size.x] = (struct cell(*)[size.x])input->grid;
        // printGrid(size, grid);

        int numElfs = numUnits(size, grid, ELF);

        // Set initial elf and goblin dists
        setDists(size, grid);

        // Test elf powers until no elfs are lost
        int elfPower = 4;
        int rounds;
        struct cell (*copy)[size.x] =
                (struct cell(*)[size.x])calloc(size.x * size.y, sizeof(struct cell));
        for (;;) {
                copyGrid(size, grid, copy);

                rounds = 0;
                for (;;) {
                        if(!completeRound(size, copy, elfPower))
                                break;
                        rounds++;
                }
                int elfsLeft = numUnits(size, copy, ELF);
                if (elfsLeft == numElfs)
                        break;

                elfPower++;
        }

        // Get battle outcome
        int hpRemaining = 0;
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        if (copy[y][x].type == ELF || copy[y][x].type == GOBLIN)
                                hpRemaining += copy[y][x].hp;
                }
        }

        int outcome = hpRemaining * rounds;

        printf("Part 2: Battle outcome = %d\n", outcome);
        free(copy);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.size = (ivec2){{getLongestLine(ll), ll->length}};
        ivec2 size = input.size;
        input.grid = calloc(size.x * size.y, sizeof(struct cell));
        struct cell (*grid)[size.x] = (struct cell(*)[size.x])input.grid;

        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;
                for (int x = 0; x < (int)strlen(str); x++) {
                        grid[y][x].hp = 0;
                        if (str[x] == '#') {
                                grid[y][x].type = WALL;
                        } else if (str[x] == '.') {
                                grid[y][x].type = OPEN;
                        } else {
                                grid[y][x].type = (str[x] == 'E' ? ELF : GOBLIN);
                                grid[y][x].hp = 200;
                        }
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day15.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day15.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day15.txt";
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

