/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Monday Sep 21, 2026 18:10:49 EDT
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
#include "../util/vector.h"
#include "../util/quicksort.h"

#define INPUT_BUFFER_SIZE 1024

typedef enum {
        UP,
        RIGHT,
        DOWN,
        LEFT
} direction;

struct cart {
        direction dir;
        ivec2 pos;
        int nextTurn;
};
typedef tal(struct cart) talcart;

struct input {
        ivec2 trackSize;
        char **tracks;
        talcart carts;
};

static bool Debug = false;
void debugp(const char *format, ...) {
        va_list args;
        va_start(args, format);
        if (Debug)
                vprintf(format, args);
        va_end(args);
}

void printTracks(ivec2 size, char tracks[][size.x]) {
        for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                        printf("%c", tracks[y][x]);
                }
                printf("\n");
        }
        printf("\n");
}

void printcarts(talcart carts) {
        const char *dirs[4] = {"UP", "RIGHT", "DOWN", "LEFT"};
        tal_for(carts, i) {
                struct cart cart = carts.array[i];
                direction next = ((cart.dir + cart.nextTurn) + 4) % 4;
                printf("Cart %lu: Dir = %s, Pos = (%d, %d), Next Dir = %s\n",
                                i, dirs[cart.dir], cart.pos.x, cart.pos.y, dirs[next]);
        }
}

talcart copyCarts(talcart carts) {
        talcart copy = tal_init();
        tal_for(carts, i) {
                tal_add(copy, carts.array[i]);
        }
        return copy;
}

// Return 1 if a > b, 0 if a == b, -1 if b > a
int cmp(void *a, void *b) {
        struct cart *cart1 = (struct cart*)a;
        struct cart *cart2 = (struct cart*)b;

        if (cart1->pos.y > cart2->pos.y)
                return 1;
        if (cart1->pos.y < cart2->pos.y)
                return -1;
        if (cart1->pos.x > cart2->pos.x)
                return 1;
        if (cart1->pos.x < cart2->pos.x)
                return -1;

        return 0;
}

void sortcarts(talcart carts) {
        quicksort(carts.array, sizeof(struct cart), 0, carts.length - 1, cmp);
}

int crash(talcart carts, int index) {
        tal_for(carts, i) {
                if ((int)i == index) continue;
                if (ivec2Eq(carts.array[index].pos, carts.array[i].pos))
                        return i;
        }
        return -1;
}

void moveCart(ivec2 size, char tracks[][size.x], struct cart *cart) {
        const ivec2 dirs[4] = {{{0, -1}}, {{1, 0}}, {{0, 1}}, {{-1, 0}}};
        cart->pos = ivec2Add(cart->pos, dirs[cart->dir]);
        char track = tracks[cart->pos.y][cart->pos.x];

        if (track == '/') {
                if (cart->dir == UP)
                        cart->dir = RIGHT;
                else if (cart->dir == RIGHT)
                        cart->dir = UP;
                else if (cart->dir == DOWN)
                        cart->dir = LEFT;
                else // LEFT
                        cart->dir = DOWN;
        } else if (track == '\\') {
                if (cart->dir == UP)
                        cart->dir = LEFT;
                else if (cart->dir == RIGHT)
                        cart->dir = DOWN;
                else if (cart->dir == DOWN)
                        cart->dir = RIGHT;
                else // LEFT
                        cart->dir = UP;
        } else if (track == '+') {
                cart->dir = ((cart->dir + cart->nextTurn) + 4) % 4;
                cart->nextTurn += 1;
                if (cart->nextTurn > 1) cart->nextTurn = -1;
        }

}

ivec2 tickCarts(ivec2 size, char tracks[][size.x], talcart carts) {
        tal_for(carts, i) {
                struct cart *cart = &carts.array[i];
                moveCart(size, tracks, cart);
                if (crash(carts, i) != -1) {
                        return cart->pos;
                }
        }
        return (ivec2){{-1, -1}};
}

// Removes crashed carts
void tickCarts2(ivec2 size, char tracks[][size.x], talcart *carts) {
        tal_for(*carts, i) {
                struct cart *cart = &carts->array[i];
                moveCart(size, tracks, cart);
                int crashedCart = crash(*carts, i);
                if (crashedCart != -1) {
                        tal_remove(*carts, crashedCart);
                        if (crashedCart < (int)i) i--;
                        tal_remove(*carts, i);
                        i--;
                }
        }
}

void part1(struct input *input) {
        ivec2 size = input->trackSize;
        char (*tracks)[size.x] = (char(*)[size.x])input->tracks;
        talcart carts = copyCarts(input->carts);
        // printTracks(size, tracks);
        // printcarts(carts);

        ivec2 crashPos;
        for (;;) {
                crashPos = tickCarts(size, tracks, carts);
                sortcarts(carts);
                if (crashPos.x != -1)
                        break;
        }

        printf("Part 1: Crash at (%d, %d)\n\n", crashPos.x, crashPos.y);
}

void part2(struct input *input) {
        ivec2 size = input->trackSize;
        char (*tracks)[size.x] = (char(*)[size.x])input->tracks;
        talcart carts = copyCarts(input->carts);
        // printTracks(size, tracks);
        // printcarts(carts);

        for (;;) {
                tickCarts2(size, tracks, &carts);
                sortcarts(carts);
                if (carts.length <= 1)
                        break;
        }
        ivec2 lastPos = carts.array[0].pos;

        printf("Part 2: Last Cart at (%d, %d)\n", lastPos.x, lastPos.y);
}

struct input parseInput(llist *ll) {
        struct input input = {0};
        input.trackSize = (ivec2){{getLongestLine(ll), ll->length}};
        ivec2 size = input.trackSize;
        input.tracks = calloc(size.x * size.y, sizeof(char));
        char (*tracks)[size.x] = (char(*)[size.x])input.tracks;


        int y = 0;
        for (llNode *cur = ll->head; cur != NULL; cur = cur->next) {
                char *str = (char*)cur->data;

                for (int x = 0; x < (int)strlen(str); x++) {
                        char c = str[x];
                        if (c == '^' || c == 'v') {
                                struct cart cart = {c == '^' ? UP : DOWN, (ivec2){{x, y}}, -1};
                                tal_add(input.carts, cart);
                                tracks[y][x] = '|';
                        } else if (c == '>' || c == '<') {
                                struct cart cart = {c == '>' ? RIGHT : LEFT, (ivec2){{x, y}}, -1};
                                tal_add(input.carts, cart);
                                tracks[y][x] = '-';
                        } else {
                                tracks[y][x] = str[x];
                        }
                }
                y++;
        }

        return input;
}

int main(int argc, char *argv[]) {
        setvbuf(stdout, NULL, _IONBF, 0);
        printf("*----------------------------------------------------------------*\n");
        printf("Running Advent of Code 2018/Day13.c\n\n");
        clock_t begin = clock();
        llist *ll;
        if (argc > 1 && strcmp(argv[1], "TEST") == 0) {
                char *file = "assets/tests/2018/Day13.txt";
                ll = getInputFileLen(file, INPUT_BUFFER_SIZE);
                Debug = true;
        } else {
                char *file = "assets/inputs/2018/Day13.txt";
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

