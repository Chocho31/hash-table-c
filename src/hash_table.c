#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static int ht_hash(const char *s, const int a, const int mod) {
    long hash = 0;
    const int len_s = strlen(s);

    for (int i = 0; i < len_s; i++) {
        hash += (long) pow(a, len_s - (i + 1)) * s[i];
        hash = hash % mod;
    }

    return (int) hash;
}

static ht_item* ht_new_item(const char *key, const char *value) {
    ht_item* item = malloc(sizeof(ht_item));

    item->key = strdup(key);
    item->value = strdup(value);

    return item;
}

static void ht_del_item(ht_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}

ht_hash_table* ht_new_hash_table() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t) ht->size, sizeof(ht_item*));

    return ht;
}

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];

        if (item != NULL) {
            ht_del_item(item);
        }
    }

    free(ht->items);
    free(ht);
}