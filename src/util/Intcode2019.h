/*************************************************
 *File----------Intcode2019.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Thursday Apr 02, 2026 14:42:19 UTC
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef INTCODE2019_H
#define INTCODE2019_H

#include "util.h"
#include "talist.h"

typedef tal(int64) talint64;

typedef struct haltmode {
        bool halt; // True: halt, False: Pause for input
        u32 index;
        int64 relBase;
} haltmode;


talint64 copyIntcode(talint64 intcode);
haltmode runIntcode(talint64 *intcode, haltmode state, talint64 *inputs, talint64 *outputs);

#endif

