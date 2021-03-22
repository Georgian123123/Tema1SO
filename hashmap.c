#include "hashmap.h"
#include <limits.h>
int hash(char *word)
{
	int hash = 0, c = 0;
	size_t i = 0;

	for (i = 0; word[i] != '\0'; i++) {
		if (isalpha(word[i]) || isdigit(word[i])) {
			c = (unsigned char)word[i];
			hash = (hash << 3) +
				(hash >> (sizeof(hash) * CHAR_BIT - 3)) + c;
		}
	}
	if (hash < 0)
		hash -= hash * 2;
	return hash % MAP_LEN;
}

MyHashMap *create(void)
{
/*
	*	Alloc memory for the hashmap, after that
	*	alloc memory for the pointers
	*/
	int i = 0, j = 0;
	MyHashMap *m = calloc(1, sizeof(MyHashMap));

	if (m == NULL)
		return NULL;

	m->table = calloc(MAP_LEN, sizeof(const struct node *));
	if (m->table == NULL) {
		free(m);
		return NULL;
	}
	for (i = 0; i < MAP_LEN; i++) {
		m->table[i] = calloc(1, sizeof(const struct node));
		if (m->table[i] == NULL) {
			for (j = 0; j < MAP_LEN; j++) {
					free(m->table[j]);
			}
			free(m->table);
			free(m);
			return NULL;
		}
		m->table[i]->key = NULL;
		m->table[i]->value = NULL;
	}
	m->table_size = 0;
	return m;
}

void put(MyHashMap *obj, char *key, char *value)
{
	int length_key = hash(key), added = 0;
	/*
	 *	If there's nothing, we do not need to clean the data stored
	 *	and put something there.
	 *	But if it is, we need to clean the data stored
	 */
	if (obj->table[length_key]->key != NULL
		&& obj->table[length_key]->value != NULL) {
		free(obj->table[length_key]->key);
		free(obj->table[length_key]->value);
		added = 1;
	}

	/*
	 * Put the data in the hashmap and increase the length of hashmap
	 */
	obj->table[length_key]->key = calloc(strlen(key) + 1, sizeof(char));
	obj->table[length_key]->value = calloc(strlen(value) + 1, sizeof(char));
	strcpy(obj->table[length_key]->key, key);
	strcpy(obj->table[length_key]->value, value);

	if (added == 0)
		obj->table_size++;

}

void *get(MyHashMap *obj, char *key)
{
	/*
	 * If there's nothing return nill
	 * otherwise return the data
	 */
	return obj->table[hash(key)] != NULL ?
			obj->table[hash(key)]->value : NULL;
	}

void removeVal(MyHashMap *obj, char *key)
{
	/*
	 *	remove the value from map
	 *  insert the new one instead
	 */
	free(obj->table[hash(key)]->key);
	obj->table[hash(key)]->key = NULL;

	free(obj->table[hash(key)]->value);
	obj->table[hash(key)]->value = NULL;
}
void free_map(MyHashMap *obj)
{
	/*
	 * Free function which go through the hashmap
	 * and delete every pointer
	 * Before that we clean the data stored in hashmap
	 * Delete the table
	 */
	int i;

	for (i = 0; i < MAP_LEN; i++) {
		if ((obj->table[i]->key != NULL &&
			obj->table[i]->value != NULL)) {
			free(obj->table[i]->key);
			free(obj->table[i]->value);
		}
		free(obj->table[i]);
	}
	free(obj->table);
	free(obj);
}
