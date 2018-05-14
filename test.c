#include "hashtable.h"
#include <stdio.h>

void show_menu();

void ht_dump(hashtable *ht) {
	struct ht_bucket *bucket;
	struct ht_entry *entry;
	for (unsigned int i = 0; i < ht->size; i++) {
		bucket = &ht->table[i];
		if (bucket->head == NULL) {
			printf("INDEX -> %u : NULL\n", i);
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

int main(int argc, char **argv) {

	hashtable *ht;

	int exit = 0;
	int option = 98;
	int nelem = 0;
	unsigned int req_size;

	printf("To start, set the table size: ");
	scanf("%d", &req_size);

	ht = ht_init(req_size);

	do {
		fflush(stdin);
		show_menu();
		// get user option
		printf("Option: ");
		scanf("%d", &option);

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
	} while (!exit);

	ht_destroy(ht);
	printf("End\n");

	return 0;
}