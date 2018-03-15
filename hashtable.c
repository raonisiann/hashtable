
#ifndef INCLUDE_HASHTABLE_H
#include "hashtable.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void show_menu();

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

void ht_dump(hashtable *ht) {
	struct ht_bucket *bucket;
	struct ht_entry *entry;
	for (int i = 0; i < ht->size; i++) {
		bucket = &ht->table[i];
		if (bucket->head == NULL) {
			printf("INDEX -> %u : NULL\n", i );
		} else {
			if (bucket->head == bucket->tail) {
				printf("INDEX -> %u : %s\n", i, bucket->head->data);
				continue;
			}
			entry = bucket->head;
			printf("INDEX -> %u : ", i);
			while (entry != NULL) {
				printf("%s, ", entry->data);
				entry = entry->next;
			}
			printf("\n");
		}
	}
}

void show_menu() {
	printf("\n------ MENU -------------------------------------\n\n");
	printf(" 1  -  Insert \n");
	printf(" 2  -  Get \n");
	printf(" 3  -  Delete \n");
	printf(" 4  -  Dump \n");
	printf(" 99 -  Exit \n");
	printf("\n------ MENU -------------------------------------\n");
}

int main(int argc, char **argv){

    hashtable *ht;

	int exit = 0;
	int option = 98;
	int nelem = 0;

    ht = ht_init(11);
   
	do {
		switch (option) {
			case 0:
				exit = 1;
				break;
			case 1:
				printf("Insert a record: ");
				char new[50];
				int index = -1;
				scanf("%s", new);
				index = ht_set(ht, new);
				if (index < 0) {
					printf("\nFail to insert record\n");
				} else {
					printf("\nSUCCESS! Inserted into position %d\n", index);
				}
				option = 99;
				break;
			case 2:
				printf("Get a record: ");
				char get[50];
				struct ht_entry *entry;
				scanf("%s", get);
				entry = ht_get(ht, get);
				if (entry == NULL) {
					printf("\nNOT FOUND\n");
				} else {
					printf("\nSUCCESS! Founded '%s'\n", entry->data);
				}
				option = 99;
				break;
			case 3:
				printf("Delete record:\n");
				option = 99;
				break;
			case 4:
				printf("Dumping records:\n");
				ht_dump(ht);
				break;
			case 99:
				exit = 1;
				break;
		}
		fflush(stdin);
		show_menu();
	// get user option
		printf("Option: ");
		scanf("%d", &option);
	} while (!exit);

    printf("End\n");

    return 0;
}