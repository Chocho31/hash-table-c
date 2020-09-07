#ifndef HASH_TABLE_HEADER
#define HASH_TABLE_HEADER

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

static int ht_hash(const char* s, const int a, const int m)
static ht_item* ht_new_item(const char *key, const char *value);
static void ht_del_item(ht_item* item);
ht_hash_table* ht_new_hash_table();
void ht_del_hash_table(ht_hash_table* ht);


#endif