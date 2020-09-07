#ifndef HASH_TABLE_HEADER_
#define HASH_TABLE_HEADER_

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size_index;
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

static int ht_generic_hash(const char* s, const int a, const int m);
static int ht_hash(const char* s, const int num_buckets, const int attempt);

static ht_item* ht_new_item(const char *key, const char *value);
static void ht_del_item(ht_item* item);

static ht_hash_table* ht_new_sized_hash_table(const int size_index);
ht_hash_table* ht_new_hash_table();
void ht_del_hash_table(ht_hash_table* ht);

static void ht_resize(ht_hash_table* ht, const int direction);

void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);

#endif