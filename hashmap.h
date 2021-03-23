#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define KEY_SIZE 100
#define VALUE_SIZE 100
#define MAP_LEN 130

	/*
	 * Data stored in hashmap
	 */
struct node {
	char *key;
	char *value;
};

	/*
	 * This si the hashmap, which contains an array of pointers
	 * and the length of hashmap
	 */
typedef struct {
	struct node **table;
	int table_size;
} MyHashMap;

	/*
	 * Functions for hashmap
	 */
int hash(char *str);
MyHashMap *create(void);
int put(MyHashMap *obj, char *key, char *value);
void *get(MyHashMap *obj, char *key);
void free_map(MyHashMap *obj);
void removeVal(MyHashMap *obj, char *key);

#endif
