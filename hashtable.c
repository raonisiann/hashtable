
#ifndef INCLUDE_HASHTABLE_H
#include "hashtable.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

hashtable *ht_init(unsigned int size){
    hashtable *ht_new = (hashtable *) malloc(sizeof(hashtable));
    if(ht_new == NULL){
        return NULL;
    }
    // allocate memory for all buckets 
    ht_new->table = (struct ht_bucket *) calloc(size, sizeof(struct ht_bucket));
    if(ht_new->table == NULL){
        return NULL;
    }

	memset(ht_new->table, NULL, sizeof(struct ht_bucket) * size);

    ht_new->size = size;
    ht_new->free = size;
    return ht_new;
}

void ht_destroy(hashtable *ht) {
	struct ht_bucket *bucket;
	struct ht_entry *entry;
	for (int i = 0; i < ht->size; i++) {
		bucket = &ht->table[i];
		if (bucket->head == NULL) {
			printf("INDEX -> %u : NULL\n", i);
		} else {
			entry = bucket->head;
			while (entry != NULL) {
				
				entry = entry->next;
			}
			printf("\n");
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

 int ht_set(hashtable *ht, char *data){
    unsigned int key, index;
    struct ht_bucket *bucket;
    struct ht_entry *entry = (struct ht_entry*)malloc(sizeof(struct ht_entry));
    
	if (ht->free == 0) {
		free(entry);
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
