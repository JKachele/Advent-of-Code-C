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
        void *key;
        void *item;
        struct ht_entry *next;
} ht_entry;

typedef struct hashtable {
        size_t size;
        ht_entry **table;
} hashtable;

hashtable ht_create(size_t size);

size_t md5_hash(const char *key, size_t ht_size);

void ht_add(hashtable *ht, size_t key_size, const void *key, void *item,
                size_t (*hash_fn)(const void *key, size_t mod));

bool ht_remove(hashtable *ht, const void *key, int (*cmp)(const void*, const void*),
                size_t (*hash_fn)(const void *key, size_t mod));

void *ht_find(hashtable *ht, const void *key, int (*cmp)(const void*, const void*),
                size_t (*hash_fn)(const void *key, size_t mod));

#endif

