
#ifndef INCLUDE_HASHTABLE_H
#include "hashtable.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

hashtable *ht_init(unsigned int size){
    hashtable *ht_new = (hashtable *) malloc(sizeof(hashtable));
    if(ht_new == NULL){
        printf("Unable to allocate memory for hash table.\n");
        return NULL;
    }
    // allocate memory for all buckets 
    ht_new->table = (struct ht_bucket *) calloc(size, sizeof(struct ht_bucket));
    if(ht_new->table == NULL){
        printf("Unable to allocate memory for hast table buckets\n");
        return NULL;
    }
    ht_new->size = size;
    ht_new->free = size;
    return ht_new;
}

unsigned int ht_key_calc(char *string){
    printf("ht_key_calc called\n");
    int size; 
    unsigned int key;

    printf("Calculating key for '%s'\n", string);
    size = strlen(string);

    for(int i = 0; i < size; i++){
        key += string[i];
        key += key << 7;
        key ^= key >> 3;
    }
    printf("Calculed key %u \n", key);
    return key;
}

unsigned int ht_set(hashtable *ht, char *data){
    unsigned int key, index;
    struct ht_bucket *bucket;
    struct ht_entry *entry = (struct ht_entry*)malloc(sizeof(struct ht_entry));
    
    entry->data = data;
    entry->next = NULL;
    key = ht_key_calc(data);
    index = key % ht->size;
    // get the bucket for indexed position
    bucket = &ht->table[index];
    if(bucket->head == NULL){
        printf("First record for index %u\n", index);
        bucket->head = entry;
        bucket->tail = entry;
    }else{
        bucket->tail->next = entry;
        bucket->tail = entry;
    }
    return index;
}

struct ht_entry *ht_get(hashtable *ht, char *data){
    printf("ht_get called\n");
    unsigned int key, index = 0;
    struct ht_entry *entry;
    struct ht_bucket *bucket;
    printf("Starting calculation\n");
    index = ht_key_calc(data);
    printf("Search for index %u", 77);
    bucket = (&ht->table[index]);
    // head and tail are iquals
    if(bucket->head == bucket->tail){
        printf("Only one registry.\n");
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

int main(int argc, char **argv){

    hashtable *ht;
    unsigned int new;

    ht = ht_init(107);
    printf("Inserted ->> %u\n", ht_set(ht, "abc"));
    printf("Inserted ->> %u\n", ht_set(ht, "idasvdacdas"));
    printf("Inserted ->> %u\n", ht_set(ht, "cdsacldalc"));
    printf("Inserted ->> %u\n", ht_set(ht, "Raoni"));

    printf("Saved --> %s\n", (ht_get(ht, "abc"))->data );
    printf("Saved --> %s\n", (ht_get(ht, "idasvdacdas"))->data );
    printf("Saved --> %s\n", (ht_get(ht, "cdsacldalc"))->data );
    printf("Saved --> %s\n", (ht_get(ht, "cdsacldalc"))->data );

    printf("End\n");

    return 0;
}