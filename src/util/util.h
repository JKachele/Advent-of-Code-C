/*************************************************
 *File----------util.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 17, 2024 08:13:18 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define max(a,b) \
    ({ __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; })

typedef struct {
        long int x;
        long int y;
} vector2;

int msleep(long msec);
void printInt(void *data);
void printLong(void *data);
int64_t ipow(int64_t base, uint8_t exp);


#endif

