#ifndef UTILITIES_H
#define UTILITIES_H
#include "hashmap.c"

#define ENOMEM 12
#define LINE_SIZE 256
#define BUFFER_SIZE 256
#define WORD_SIZE 64
#define NR_DIRECTORIES 64
#define LENGTH_NAME_DIRECTORY 63
int size_directory;

int check_params(int number_args, char **arguments, MyHashMap *obj,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

int open_input(char *input_file, MyHashMap *obj,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

int read_from_file(FILE *fp, MyHashMap *obj, char *buffer, char *input_file,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

void resolve_define(char *text, MyHashMap *obj, FILE *fp);

void param_I(int number_args, char **arguments, MyHashMap *obj, char *arg,
		int len, int *i,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

int param_D(int number_args, char **arguments, MyHashMap *obj, char *arg,
			int len, int *i);
char *strnstr(const char *s, const char *find, size_t slen);
void removeChar(char *str, char garbage);
void put_values_text(MyHashMap *obj, char *line, char *buffer);
void help_ifdef(char *text, MyHashMap *obj, FILE *fp, char *buffer);
void resolve_ifdef_ifndef(char *text, MyHashMap *obj, FILE *fp,
						char *buffer, int type);
int param_O(int number_args, char **arguments, MyHashMap *obj, char *arg,
			int len, int *i);
int resolve_include(char *line, MyHashMap *obj, char *buffer, FILE *fp,
		char *input_file,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);
int read_from_file(FILE *fp, MyHashMap *obj, char *buffer,
		char *input_file,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

#endif
