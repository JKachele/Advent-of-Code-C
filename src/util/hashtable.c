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

hashtable ht_create(size_t size) {
        assert(size != 0);

        hashtable ht = {0};
        ht.size = size;
        ht.table = calloc(size, sizeof(ht_entry*));
        return ht;
}

size_t hash_fn(const char *key, size_t ht_size, size_t seed) {
        assert(key != NULL);

        size_t hash = seed ^ 2166136261UL;
        for (int i = 0; key[i] != '\0'; i++) {
                hash = (hash ^ key[i]) % ht_size;
                hash = (hash * 16777619) % ht_size;
        }
        return hash;
}

size_t md5_hash(const char *key, size_t ht_size) {
        return md5_64(key) % ht_size;
}

void ht_add(hashtable *ht, const char *key, void *item) {
        assert(ht != NULL && key != NULL && item != NULL);

        size_t hash = md5_hash(key, ht->size);
        ht_entry *entry = malloc(sizeof(ht_entry));
        entry->key = malloc(strlen(key) + 1);
        strcpy(entry->key, key);
        entry->item = item;
        entry->next = NULL;

        if (ht->table[hash] != NULL) {
                entry->next = ht->table[hash];
        }
        ht->table[hash] = entry;
}

bool ht_remove(hashtable *ht, const char *key) {
        assert(ht != NULL && key != NULL);

        size_t hash = md5_hash(key, ht->size);
        if (ht->table[hash] == NULL) { return false; }

        ht_entry *prev = NULL;
        ht_entry *cur = ht->table[hash];
        while (cur != NULL) {
                if (strcmp(cur->key, key) == 0) {
                        if (prev == NULL) {
                                ht->table[hash] = cur->next;
                        } else {
                                prev->next = cur->next;
                        }
                        free(cur->key);
                        free(cur->item);
                        free(cur);
                        return true;
                }
                prev = cur;
                cur = cur->next;
        }
        return false;
}

void *ht_find(hashtable *ht, const char *key) {
        assert(ht != NULL && key != NULL);

        size_t hash = md5_hash(key, ht->size);
        if (ht->table[hash] == NULL) { return NULL; }

        ht_entry *cur = ht->table[hash];
        while (cur != NULL) {
                if (strcmp(cur->key, key) == 0) {
                        return cur;
                }
                cur = cur->next;
        }

        return NULL;
}

