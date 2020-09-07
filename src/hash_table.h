#ifndef HASH_TABLE_HEADER
#define HASH_TABLE_HEADER

typedef struct {
    char *key;
    char *value;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item **items;
} ht_hash_table;

#endif