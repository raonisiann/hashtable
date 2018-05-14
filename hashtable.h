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
    unsigned int size;
    unsigned int free;
	// index for current position on ht_prime_numbers
	unsigned int i_prime_arr; 
};

typedef struct hash_table hashtable;

unsigned int ht_key_calc(char *data);
int ht_set(hashtable *ht, char *data);
struct ht_entry *ht_get(hashtable *ht, char *data);
hashtable *ht_init(unsigned int size);
void ht_resize_table(hashtable *ht);
void ht_reindex(hashtable *ht);
unsigned int ht_find_best_size(unsigned int size);
void ht_destroy(hashtable *ht);
void ht_dump(hashtable *ht);