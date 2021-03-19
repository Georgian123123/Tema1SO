#ifndef UTILITIES_H
#define UTILITIES_H
#include "hashmap.c"
#include <errno.h>

#define LINE_SIZE 256
#define BUFFER_SIZE 8192
#define WORD_SIZE 512
#define NR_DIRECTORIES 512
#define LENGTH_NAME_DIRECTORY 1024

static int size_directory;
void check_params(int number_args, char **arguments, MyHashMap *obj,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

int open_input(char *input_file, MyHashMap *obj,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

int read_from_file(FILE *fp, MyHashMap *obj, char *buffer, char *input_file,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

void resolve_define(char *text, MyHashMap *obj, FILE *fp);

void param_I(int number_args, char **arguments, MyHashMap *obj, char *arg,
		int len, int *i,
		char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY]);

void param_D(int number_args, char **arguments, MyHashMap *obj, char *arg,
			int len, int *i);
#endif
