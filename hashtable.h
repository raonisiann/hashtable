#define INCLUDE_HASHTABLE_H

struct ht_entry {  
    struct ht_entry *next;
    char *data; // could be any type of data
};

struct ht_bucket {
    struct ht_entry *head;
    struct ht_entry *tail;
    unsigned int index;
};

struct hash_table {
    struct ht_bucket *table;
    int size;
    int free;
};

typedef struct hash_table hashtable;

unsigned int ht_key_calc(char *data);
unsigned int ht_set(hashtable *ht, char *data);
struct ht_entry *ht_get(hashtable *ht, char *data);
hashtable *ht_init(unsigned int size);