#include "hashtable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Array with prime numbers
// This will be one of the possible sizes for a hashtable
unsigned int ht_prime_numbers[] = { 5, 11, 19, 53, 107, 223, 463, 983, 1979, 3907, 7963, 16229, 32531, 65407, 130987, 262237, 524521, 1048793, 2097397, 4194103, 8388857, 16777447, 33554201, 67108961, 134217487, 268435697, 536870683, 1073741621, 2147483399 };

hashtable *ht_init(unsigned int size){
	unsigned int ht_prime_index; 
    hashtable *ht_new = (hashtable *) malloc(sizeof(hashtable));
    if(ht_new == NULL){
        return NULL;
    }
	ht_prime_index = ht_find_best_size(size);
	printf("table size %u\n", ht_prime_numbers[ht_prime_index]);
    // allocate memory for all buckets 
    ht_new->table = (struct ht_bucket *) malloc(sizeof(struct ht_bucket) * ht_prime_numbers[ht_prime_index]);
    if(ht_new->table == NULL){
        return NULL;
    }

	memset(ht_new->table, NULL, sizeof(struct ht_bucket) * ht_prime_numbers[ht_prime_index]);

    ht_new->size = ht_prime_numbers[ht_prime_index];
    ht_new->free = ht_prime_numbers[ht_prime_index];
	ht_new->i_prime_arr = ht_prime_index;
    return ht_new;
}

void ht_destroy(hashtable *ht) {
	struct ht_bucket *bucket;
	struct ht_entry *entry;
	for (unsigned int i = 0; i < ht->size; i++) {
		bucket = &ht->table[i];
		if (bucket->head != NULL) {
			entry = bucket->head;
			while (entry != NULL) {
				entry = entry->next;
			}
		}
	}
}

unsigned int ht_key_calc(char *string){
    int size; 
    unsigned int key = 0;

    size = strlen(string);

    for(int i = 0; i < size; i++){
        key += string[i];
        key += key << 7;
        key ^= key >> 3;
    }
    return key;
}

unsigned int ht_find_best_size(unsigned int size) {
	unsigned int arr_size;
	arr_size = sizeof(ht_prime_numbers) / sizeof(unsigned int);
	for (unsigned int i = 0; i < arr_size; i++) {
		if (ht_prime_numbers[i] > size) {
			return i;
		}
	}
	// return the last position 
	return arr_size - 1;
}

void ht_resize_table(hashtable *ht) {
	printf("This table needs a resize\n");

	ht->i_prime_arr = ht->i_prime_arr + 1;
	ht->free = (ht->free - ht->size) - ht_prime_numbers[ht->i_prime_arr];
	ht->size = ht_prime_numbers[ht->i_prime_arr];
	
	printf("new table size %u\n", ht_prime_numbers[ht->i_prime_arr]);
	// reallocate memory for bucket struct
	ht->table = (struct ht_bucket *) realloc(ht->table, sizeof(struct ht_bucket) * ht->size);
	if (ht->table == NULL) {
		exit(-1); // out of memory
	}

	for (int i = ht_prime_numbers[ht->i_prime_arr - 1]; i < ht->size; i++) {
		memset(&ht->table[i], NULL, sizeof(struct ht_bucket*));
	}

	ht_reindex(ht);
}


void ht_reindex(hashtable *ht) {
	printf("Start reindex\n");
	struct ht_bucket *bucket;
	struct ht_entry *entry;
	int cur = -1;
	for (unsigned int i = 0; i < ht->size; i++) {
		bucket = &ht->table[i];
		if (bucket->head != NULL) {
			entry = bucket->head;
			while (entry != NULL) {
				cur = ht_set(ht, entry->data);
				if (cur != i) {
					//free(entry->data);
				}
				//printf("%s, ", entry->data);
				entry = entry->next;
			}
		}
	}
}

 int ht_set(hashtable *ht, char *data){
    unsigned int key, index;
    struct ht_bucket *bucket;
    struct ht_entry *entry = (struct ht_entry*)malloc(sizeof(struct ht_entry));
    
	if (ht->free == 0) {
		ht_resize_table(ht);
		//free(entry);
		return -1;
	}

	entry->data = (char *)malloc(sizeof(data));
	strcpy(entry->data, data);
    entry->next = NULL;
    key = ht_key_calc(data);
    index = key % ht->size;
    // get the bucket for indexed position
    bucket = &ht->table[index];
    if(bucket->head == NULL){
        bucket->head = entry;
        bucket->tail = entry;
    }else{
        bucket->tail->next = entry;
        bucket->tail = entry;
    }
	ht->free = ht->free - 1;
    return index;
}

struct ht_entry *ht_get(hashtable *ht, char *data){
    unsigned int index = 0;
    struct ht_entry *entry;
    struct ht_bucket *bucket;
    index = ht_key_calc(data) % ht->size;
    bucket = (&ht->table[index]);
    // head and tail are iquals
    if(bucket->head == bucket->tail){
        return bucket->head;
    }
    entry = bucket->head;
    while(entry->next != NULL){
        if(entry->data == data){
            break;
        }
        entry = entry->next;
    }
    return entry;
}
