/*************************************************
 *File----------util.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday May 08, 2024 10:51:16 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "util.h"

#include <time.h>
#include <errno.h>

// int msleep(long msec) {
//     struct timespec ts;
//     int res;

//     if (msec < 0) {
//         errno = EINVAL;
//         return -1;
//     }

//     ts.tv_sec = msec / 1000;
//     ts.tv_nsec = (msec % 1000) * 1000000;

//     do {
//         res = nanosleep(&ts, &ts);
//     } while (res && errno == EINTR);

//     return res;
// }

