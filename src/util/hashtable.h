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

typedef size_t (*ht_hash)(const void *key, size_t mod);
typedef int (*ht_cmp)(const void*, const void*);

typedef enum {
        HT_SUCCESS,
        HT_ENTRY_EXISTS,
        HT_ENTRY_NOT_FOUND
} ht_error;

typedef struct ht_entry {
        void *key;
        void *item;
        struct ht_entry *next;
} ht_entry;

typedef struct hashtable {
        size_t capacity;
        size_t size;
        float load_factor;
        size_t threshold;
        ht_entry **table;
        ht_hash hash_fn;
        ht_cmp cmp_fn;
} hashtable;

hashtable ht_create(size_t size, ht_hash hash_fn, ht_cmp cmp_fn);

size_t md5_hash(const char *key, size_t ht_size);

ht_error ht_add(hashtable *ht, size_t key_size, const void *key, void *item);

ht_error ht_remove(hashtable *ht, const void *key);

ht_entry* ht_find(hashtable *ht, const void *key);

#endif

