/*************************************************
 *File----------Day13.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Jan 09, 2025 17:49:15 EST
 *License-------GNU GPL-3.0
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../util/linkedlist.h"
#include "../util/inputFile.h"
#include "../util/util.h"

#define EPSILON 0.00001

#define TOKENS_A 3
#define TOKENS_B 1

// Checks if floating point value is within EPSILON of an int
bool doubleIsInt(double f) {
        long i = (long)f;
        double diff = f - i;
        if (diff < 0)
                diff = -1 * diff;
        return diff < EPSILON;
}

bool doubleEq(double a, double b) {
        double diff = a - b;
        if (diff < 0)
                diff = -1 * diff;
        return diff < EPSILON;
}

// Calculates determinant of 2x2 matrix a1 b1
//                                      a2 b2
double determinant(double a1, double b1, double a2, double b2) {
        return a1 * b2 - a2 * b1;
}

// Solve systems of equations: Aax + Bbx = px, Aay + Bby = py
// Using Cramers rule: if A and B are integer, prize is possible
long getTokens(vector2 a, vector2 b, vector2 p) {
        // Get main determinant to check if single solution exixts
        double det = determinant(a.x, b.x, a.y, b.y);
        if (doubleEq(det, 0)) {
                printf("Determinant = 0!\n");
                return -1;
        }

        // If single solution exixts, use cramers rule to get solution
        double detA = determinant(p.x, b.x, p.y, b.y);
        double detB = determinant(a.x, p.x, a.y, p.y);
        double fA = detA / det;
        double fB = detB / det;
        // printf("A = %f, B = %f\t", fA, fB);


        // Check if solution is both integers
        if (doubleIsInt(fA) && doubleIsInt(fB)) {
                // printf("Possible!\n");
                return TOKENS_A * (long)fA + TOKENS_B * (long)fB;
        } else {
                // printf("Not Possible!\n");
                return 0;
        }
}

void part1(llist *ll) {
        int totalTokens = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                // Get Button A
                char str1[BUFFER_SIZE];
                strncpy(str1, (char*)current->data, BUFFER_SIZE);
                vector2 a;
                strtok(str1, "+");      // Skip to after the first '+'
                char *ax = strtok(NULL, ",");
                strtok(NULL, "+");      // Skip past the ", Y+"
                char *ay = strtok(NULL, "");
                a.x = strtol(ax, (char**)NULL, 10);
                a.y = strtol(ay, (char**)NULL, 10);

                // Get Button B
                current = current->next;
                if (current == NULL)
                        break;
                char str2[BUFFER_SIZE];
                strncpy(str2, (char*)current->data, BUFFER_SIZE);
                vector2 b;
                strtok(str2, "+");      // Skip to after the first '+'
                char *bx = strtok(NULL, ",");
                strtok(NULL, "+");      // Skip past the ", Y+"
                char *by = strtok(NULL, "");
                b.x = strtol(bx, (char**)NULL, 10);
                b.y = strtol(by, (char**)NULL, 10);

                // Get Prize Location
                current = current->next;
                if (current == NULL)
                        break;
                char str3[BUFFER_SIZE];
                strncpy(str3, (char*)current->data, BUFFER_SIZE);
                vector2 p;
                strtok(str3, "=");      // Skip to after the first '='
                char *px = strtok(NULL, ",");
                strtok(NULL, "=");      // Skip past the ", Y="
                char *py = strtok(NULL, "");
                p.x = strtol(px, (char**)NULL, 10);
                p.y = strtol(py, (char**)NULL, 10);

                totalTokens += getTokens(a, b, p);

                // Skip blank line
                current = current->next;
                if (current == NULL)
                        break;
                current = current->next;
        }
        printf("Part 1: Total Tokens: %d\n\n", totalTokens);
}

void part2(llist *ll) {
        long totalTokens = 0;

        llNode *current = ll->head;
        while(current != NULL) {
                // Get Button A
                char str1[BUFFER_SIZE];
                strncpy(str1, (char*)current->data, BUFFER_SIZE);
                vector2 a;
                strtok(str1, "+");      // Skip to after the first '+'
                char *ax = strtok(NULL, ",");
                strtok(NULL, "+");      // Skip past the ", Y+"
                char *ay = strtok(NULL, "");
                a.x = strtol(ax, (char**)NULL, 10);
                a.y = strtol(ay, (char**)NULL, 10);

                // Get Button B
                current = current->next;
                if (current == NULL)
                        break;
                char str2[BUFFER_SIZE];
                strncpy(str2, (char*)current->data, BUFFER_SIZE);
                vector2 b;
                strtok(str2, "+");      // Skip to after the first '+'
                char *bx = strtok(NULL, ",");
                strtok(NULL, "+");      // Skip past the ", Y+"
                char *by = strtok(NULL, "");
                b.x = strtol(bx, (char**)NULL, 10);
                b.y = strtol(by, (char**)NULL, 10);

                // Get Prize Location
                current = current->next;
                if (current == NULL)
                        break;
                char str3[BUFFER_SIZE];
                strncpy(str3, (char*)current->data, BUFFER_SIZE);
                vector2 p;
                strtok(str3, "=");      // Skip to after the first '='
                char *px = strtok(NULL, ",");
                strtok(NULL, "=");      // Skip past the ", Y="
                char *py = strtok(NULL, "");
                p.x = strtol(px, (char**)NULL, 10) + 10000000000000;
                p.y = strtol(py, (char**)NULL, 10) + 10000000000000;
                // printf("%ld, %ld\n", p.x, p.y);

                long tokens = getTokens(a, b, p);
                totalTokens += tokens;

                // Skip blank line
                current = current->next;
                if (current == NULL)
                        break;
                current = current->next;
        }
        printf("Part 2: Total Tokens: %ld\n\n", totalTokens);
}

int main(int argc, char *argv[]) {
        llist *ll = getInputFile("assets/2024/Day13.txt");
        // llist *ll = getInputFile("assets/test.txt");
        // llist_print(ll, printInput);

        part1(ll);
        part2(ll);

        return 0;
}
