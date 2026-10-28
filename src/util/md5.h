/*************************************************
 *File----------md5.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Oct 14, 2026 10:57:05 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef MD5_H
#define MD5_H

#include <stdint.h>

void md5(const char *input, uint8_t result[16]);

uint64_t md5_64(const char *input);
uint32_t md5_32(const char *input);

#endif

