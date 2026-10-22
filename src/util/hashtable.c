/*************************************************
 *File----------hashtable.c
 *Project-------Advent-of-Code-C
 *Author--------Justin Kachele
 *Created-------Wednesday Oct 14, 2026 09:34:16 EDT
 *License-------GNU GPL-3.0
 ************************************************/

#include "hashtable.h"
#include "md5.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

static const hashtable DefaultTable = {
        .capacity = 16,
        .size = 0,
        .load_factor = 0.75f,
        .threshold = 12,
        .table = NULL,
        .hash_fn = NULL,
        .cmp_fn = NULL
};

hashtable ht_create(size_t capacity, ht_hash hash_fn, ht_cmp cmp_fn) {
        assert(hash_fn != NULL);
        assert(cmp_fn != NULL);

        hashtable ht = DefaultTable;

        if (capacity != 0) {
                ht.capacity = capacity;
                ht.threshold = (size_t)(capacity * ht.load_factor);
        }
        ht.table = calloc(ht.capacity, sizeof(ht_entry*));
        ht.hash_fn = hash_fn;
        ht.cmp_fn = cmp_fn;
        return ht;
}

size_t md5_hash(const char *key, size_t ht_size) {
        return md5_64(key) % ht_size;
}

static void ht_rehash(hashtable *ht) {
        // Make new hashtable with double capacity
        hashtable new = *ht;
        new.capacity *= 2;
        new.threshold = (int)(new.capacity * new.load_factor);
        new.table = calloc(new.capacity, sizeof(ht_entry*));

        // For each entry, calculate new hash and insert into new table
        for (size_t i = 0; i < ht->capacity; i++) {
                ht_entry *cur = ht->table[i];
                while (cur != NULL) {
                        ht_entry *next = cur->next;
                        size_t hash = new.hash_fn(cur->key, new.capacity);

                        if (new.table[hash] != NULL) {
                                cur->next = new.table[hash];
                        } else {
                                cur->next = NULL;
                        }
                        new.table[hash] = cur;

                        cur = next;
                }
        }

        // Free old table
        free(ht->table);
        *ht = new;
}

ht_error ht_add(hashtable *ht, size_t key_size, const void *key, void *item) {
        assert(ht != NULL && key != NULL && item != NULL);

        ht_entry *entry = ht_find(ht, key);
        if (entry != NULL) {
                free(entry->item);
                entry->item = item;
                return HT_ENTRY_EXISTS;
        }

        if (ht->size >= ht->threshold) {
                ht_rehash(ht);
        }

        size_t hash = ht->hash_fn(key, ht->capacity);
        ht_entry *newEntry = malloc(sizeof(ht_entry));
        newEntry->key = malloc(key_size);
        memcpy(newEntry->key, key, key_size);
        newEntry->item = item;
        newEntry->next = NULL;

        if (ht->table[hash] != NULL) {
                newEntry->next = ht->table[hash];
        }
        ht->table[hash] = newEntry;
        ht->size++;
        return HT_SUCCESS;
}

ht_error ht_remove(hashtable *ht, const void *key) {
        assert(ht != NULL && key != NULL);

        size_t hash = ht->hash_fn(key, ht->capacity);
        if (ht->table[hash] == NULL) { return HT_ENTRY_NOT_FOUND; }

        ht_entry *prev = NULL;
        ht_entry *cur = ht->table[hash];
        while (cur != NULL) {
                if (ht->cmp_fn(cur->key, key) == 0) {
                        if (prev == NULL) {
                                ht->table[hash] = cur->next;
                        } else {
                                prev->next = cur->next;
                        }
                        free(cur->key);
                        free(cur->item);
                        free(cur);
                        ht->size--;
                        return HT_SUCCESS;
                }
                prev = cur;
                cur = cur->next;
        }
        return HT_ENTRY_NOT_FOUND;
}

ht_entry *ht_find(hashtable *ht, const void *key) {
        assert(ht != NULL && key != NULL);

        size_t hash = ht->hash_fn(key, ht->capacity);
        if (ht->table[hash] == NULL) { return NULL; }

        ht_entry *cur = ht->table[hash];
        while (cur != NULL) {
                if (ht->cmp_fn(cur->key, key) == 0) {
                        return cur;
                }
                cur = cur->next;
        }

        return NULL;
}

