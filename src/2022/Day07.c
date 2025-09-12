/*************************************************
 *File----------Day07.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Tuesday Mar 25, 2025 09:20:24 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../lib/tllist.h"
#include "../util/util.h"

#define MAX_SIZE 40000000

static const bool DEBUG = false;
void debugP(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (DEBUG)
                vprintf(format, args);
        va_end(args);
}

typedef struct {
        uint64 hash;
        uint64 size;
} directory;

typedef tll(directory) tlldir;
typedef tll(uint64) tllhash;

void printDirs(tlldir dirs) {
        if (!DEBUG) return;
        tll_foreach(dirs, it) {
                directory d = it->item;
                printf("%lu: %lu\n", d.hash, d.size);
        }
}

bool isCommand(llNode *line) {
        char *l = (char*)(line->data);
        return (l[0] == '$');
}

uint64 parseDir(const char *line) {
        return computeHash(line + 4);
}

uint64 parseFile(char *line) {
        return strtol(strtok(line, " "), (char**)NULL, 10);
}

uint64 parseCD(const char *line) {
        return computeHash(line + 5);
}

int getDirSizes(llNode **current, tlldir *dirs, uint64 dirHash) {
        directory curDir = {.hash = dirHash, .size = 0};
        tllhash availDirs = tll_init();

        // Make sure first command after cd is ls
        if (strcmp((char*)(*current)->data, "$ ls") != 0) {
                printf("Command not ls!\n");
                return 0;
        }
        *current = (*current)->next;
        while (*current != NULL && !isCommand(*current)) {
                char line[BUFFER_SIZE];
                strncpy(line, (char*)(*current)->data, BUFFER_SIZE);

                if (line[0] == 'd') {
                        uint64 dir = parseDir(line);
                        debugP("Dir found with hash: %lu\n", dir);
                        tll_push_back(availDirs, dir);
                } else {
                        uint64 fileSize = parseFile(line);
                        debugP("File found with size: %lu\n", fileSize);
                        curDir.size += fileSize;
                }

                *current = (*current)->next;
        }
        if (*current == NULL) {
                tll_push_back(*dirs, curDir);
                return curDir.size;
        }

        while (*current != NULL) {
                char line[BUFFER_SIZE];
                strncpy(line, (char*)(*current)->data, BUFFER_SIZE);
                debugP("%s\n", line);
                uint64 dir = parseCD(line);

                *current = (*current)->next;
                if (dir == computeHash("..")) break;
                curDir.size += getDirSizes(current, dirs, dir);
        }

        tll_push_back(*dirs, curDir);
        return curDir.size;
}

void part1_2(llist *ll) {
        tlldir dirs = tll_init();
        uint64 rootHash = computeHash("/");
        uint64 rootSize = getDirSizes(&(ll->head->next), &dirs, rootHash);
        printDirs(dirs);
        
        uint64 smallestDirs = 0;
        tll_foreach(dirs, it) {
                directory d = it->item;
                if (d.size <= 100000)
                        smallestDirs += d.size;
        }

        printf("Part 1: Total Size under 100000: %lu\n\n", smallestDirs);

        if (rootSize < MAX_SIZE) {
                printf("Already enough space!\n");
                return;
        }

        uint64 neededSpace = rootSize - MAX_SIZE;
        
        // Get smallest dir with enough size
        uint64 dirToRemove = UINT64_MAX;
        tll_foreach(dirs, it) {
                directory d = it->item;
                if (d.size >= neededSpace && d.size < dirToRemove)
                        dirToRemove = d.size;
        }

        printf("Part 2: Size of dir to remove: %lu\n\n", dirToRemove);
}

int main(int argc, char *argv[]) {
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                ll = getInputFile("assets/tests/2022/Day07.txt");
        } else {
                ll = getInputFile("assets/inputs/2022/Day07.txt");
        }
        // llist_print(ll, printInput);

        clock_t parse = clock();
        part1_2(ll);
        clock_t pt1 = clock();

        double parseTime = ((double)(parse - begin) / CLOCKS_PER_SEC) * 1000;
        double pt1Time = ((double)(pt1 - parse) / CLOCKS_PER_SEC) * 1000;
        printf("Execution Time (ms) - Input Parse: %f, Solve: %f\n", 
                        parseTime, pt1Time);

        return 0;
}

