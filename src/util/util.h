/*************************************************
 *File----------util.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Apr 17, 2024 08:13:18 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef UTIL_H
#define UTIL_H

#define max(a,b) \
    ({ __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; })

int msleep(long msec);


#endif

