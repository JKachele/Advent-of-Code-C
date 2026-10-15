/*************************************************
 *File----------hashtable.h
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Oct 14, 2026 09:34:14 EDT
 *License-------GNU GPL-3.0
 ************************************************/
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct ht_entry {
        char *key;
        void *item;
        struct ht_entry *next;
} ht_entry;

typedef struct hashtable {
        size_t size;
        ht_entry **table;
} hashtable;

hashtable ht_create(size_t size);

size_t hash_fn(const char *key, size_t hash_size, size_t seed);

void ht_add(hashtable *ht, const char *key, void *item);

bool ht_remove(hashtable *ht, const char *key);

void *ht_find(hashtable *ht, const char *key);

#endif

