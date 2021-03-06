#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"
#include "prime.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};

static const int HT_PRIME_1 = 151;
static const int HT_PRIME_2 = 163;

static int ht_generic_hash(const char *s, const int a, const int mod) {
    long hash = 0;
    const int len_s = strlen(s);

    for (int i = 0; i < len_s; i++) {
        hash += (long) pow(a, len_s - (i + 1)) * s[i];
        hash = hash % mod;
    }

    return (int) hash;
}

static int ht_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_generic_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_generic_hash(s, HT_PRIME_2, num_buckets);

    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
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

static ht_hash_table* ht_new_sized_hash_table(const int size_index) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size_index = size_index;

    const int base_size = 50 << ht->size_index;

    ht->count = 0;
    ht->size = next_prime(base_size);
    ht->items = calloc((size_t) ht->size, sizeof(ht_item*));

    return ht;
}

ht_hash_table* ht_new_hash_table() {
    return ht_new_sized_hash_table(0);
}

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];

        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_del_item(item);
        }
    }

    free(ht->items);
    free(ht);
}

static void ht_resize(ht_hash_table* ht, const int direction) {
    const int new_size_index = ht->size_index + direction;

    if (new_size_index < 0) {
        return;
    }

    ht_hash_table* new_ht = ht_new_sized_hash_table(new_size_index);

    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];

        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->size_index = new_ht->size_index;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    // Resize if load > 70
    const int load = ht->count * 100 / ht->size;

    if (load > 70) {
        ht_resize(ht, 1);
    }

    ht_item* item = ht_new_item(key, value);

    int index = ht_hash(item->key, ht->size, 0);
    int i = 1;

    ht_item* cur_item = ht->items[index];

    while(cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
        if (strcmp(cur_item->key, key) == 0) {
            ht_del_item(cur_item);
            ht->items[index] = item;
            return;
        }

        index = ht_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }

    ht->items[index] = item;
    ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_hash(key, ht->size, 0);
    int i = 1;

    ht_item* item = ht->items[index];

    while(item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }

        index = ht_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }

    return NULL;
}

void ht_delete(ht_hash_table* ht, const char* key) {
    // Resize if load < 0.1
    const int load = ht->count * 100 / ht->size;
    
    if (load < 10) {
        ht_resize(ht, -1);
    }

    int index = ht_hash(key, ht->size, 0);
    int i = 1;

    ht_item* item = ht->items[index];

    while(item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }

        index = ht_hash(key, ht->size, 1);
        item = ht->items[index];
        i++;
    }

    ht->count--;
}