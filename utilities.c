#include "utilities.h"

/*
 *@params
 *number_args - number of arguments from the command line
 * arguments - arguments from the command line
 * obj - hashmap
 * arg - the current argument
 * len - length of the argument
 * i - position of the argument
 *
 * @value
 * the value of this function is splitting the key and value from
 * the define directive
 *
 * This comment should be for the next functions like: param_O
 * param_I
 */
int param_D(int number_args, char **arguments, MyHashMap *obj, char *arg,
		int len, int *i)
{
	int new_pos = 0, pos = 0, val_pos = 0;
	char name_define[256] = {0};
	char val_define[256] = {0};

	strncpy(name_define, "", 256);
	strncpy(val_define, "", 256);
	if (len != 2) {
		val_pos = 0;
		pos = 0;
		for (new_pos = 2; new_pos < len; new_pos++) {
			if (arg[new_pos] != ' ' && arg[new_pos] != '=')
				name_define[pos++] = arg[new_pos];
			else if (arg[new_pos] == '=')
				break;
		}
		name_define[pos] = '\0';

		for (++new_pos; new_pos < len; new_pos++) {
			if (arg[new_pos] != ' ')
				val_define[val_pos++] = arg[new_pos];
		}
		val_define[val_pos] = '\0';
	} else if (len == 2) {
		arg = arguments[++(*i)];
		len = strlen(arg);
		val_pos = 0;
		pos = 0;
		for (new_pos = 0; new_pos < len; new_pos++) {
			if (arg[new_pos] != ' ' && arg[new_pos] != '=')
				name_define[pos++] = arg[new_pos];
			else if (arg[new_pos] == '=')
				break;
		}
		name_define[pos] = '\0';

		for (++new_pos; new_pos < len; new_pos++) {
			if (arg[new_pos] != ' ')
				val_define[val_pos++] = arg[new_pos];
		}
		val_define[val_pos] = '\0';
	}
	if (strncmp(val_define, "", 1) == 0) {
		if (put(obj, name_define, "") == 0)
			return 0;
	} else {
		if (put(obj, name_define, val_define) == 0)
			return 0;
	}

	strncpy(name_define, "", 256);
	strncpy(val_define, "", 256);
	return 1;
}

int param_O(int number_args, char **arguments, MyHashMap *obj, char *arg,
			int len, int *i)
{
	int new_pos = 0;
	char value[LINE_SIZE] = {0};

	new_pos = 0;
	memset(value, 0, LINE_SIZE);

	if (len == 2) {
		strncpy(value, arguments[++*i], LINE_SIZE);
	} else {
		for (new_pos = 2; new_pos < len; new_pos++)
			value[new_pos - 2] = arg[new_pos];
		value[new_pos++] = '\0';
	}
	if (put(obj, "output", value) == 0)
		return 0;
	return 1;
}

void param_I(int number_args, char **arguments, MyHashMap *obj, char *arg,
		int len, int *i,
		char directory[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY])
{
	int new_pos = 0, pos = 0;
	char name_define[256] = {0};
	char val_define[256] = {0};

	strncpy(name_define, "", 256);
	strncpy(val_define, "", 256);

	if (len != 2) {
		for (new_pos = 3; new_pos < len; new_pos++)
			val_define[pos++] = arg[new_pos];
	} else if (len == 2) {
		arg = arguments[++(*i)];
		len = strlen(arg);
		for (new_pos = 0; new_pos < len; new_pos++)
			val_define[pos++] = arg[new_pos];
	}

	val_define[pos] = '\0';
	strncpy(directory[size_directory++], val_define, 256);
	strncpy(val_define, "", 256);
}

/*
 * @params
 * number_args - number of arguments
 * arguments - the arguments from the command line
 * directories - the directories included from the -I param
 *
 * @value
 * The value of this function is:
 * separate each param from the command line and extract the values
 */
int check_params(int number_args, char **arguments,
			MyHashMap *obj,
			char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY])
{
	int len = 0, i = 0;
	char *arg = NULL;

	for (i = 1; i < number_args; i++) {
		arg = arguments[i];
		len = strlen(arg);
		if (strncmp(arg, "-D", 2) == 0) {
			if (param_D(number_args, arguments,
				obj, arg, len, &i) == 0)
				return 0;
		} else if (strncmp(arg, "-I", 2) == 0) {
			param_I(number_args, arguments, obj,
					arg, len, &i, directories);
		} else if (strncmp(arg, "-o", 2) == 0) {
			if (param_O(number_args, arguments,
					obj, arg, len, &i) == 0)
				return 0;
		} else {
			if (strstr(arg, ".out")) {
				if (put(obj, "output", arg) == 0)
					return 0;
			} else {
				if (put(obj, "input", arg) == 0)
					return 0;
			}
		}
	}
	return 1;
}

/*
 * @params
 * str - the string where the garbage character will be removed
 * garbage - character that will be removed
 *
 * @value
 * The value of this function is that:
 * It will remove from the str all the garbage characters
 */
void removeChar(char *str, char garbage)
{
	char *src = NULL;
	char *dst = NULL;

	for (src = dst = str; *src != '\0'; src++) {
		*dst = *src;
		if (*dst != garbage)
			dst++;
	}
	*dst = '\0';
}

/*
 * @params
 * text - the string to be splitted
 * obj - the hashmap
 * fp - the file descriptor from the input file
 *
 * @value
 * The value of this function is:
 * at the final of the function in the map will be the
 * key - value object, from the define directive
 */
void resolve_define(char *text, MyHashMap *obj, FILE *fp)
{
	char *token = NULL;
	char *key = NULL;
	char *ans = NULL;
	char auxiliar_space[LINE_SIZE] = {0};
	char line[LINE_SIZE] = {0};
	char value[LINE_SIZE] = {0};
	int len = 0;
	int find = 0;

	memset(auxiliar_space, 0, LINE_SIZE);
	memset(line, 0, LINE_SIZE);
	memset(value, 0, LINE_SIZE);
	token = strtok(text, " ");
	key = token;
	token = strtok(NULL, " ");

	while (token != NULL && strncmp(token, "\\", 2) != 0) {
		ans = (char *)get(obj, token);
		find = 0;
		while (ans) {
			if (ans) {
				strncat(auxiliar_space, ans, LINE_SIZE);
				ans = (char *)get(obj, ans);
				find = 1;
			} else {
				break;
			}
		}
		if (!find)
			strncat(auxiliar_space, token, LINE_SIZE);

		token = strtok(NULL, " ");

		if (token != NULL)
			strncat(auxiliar_space, " ", 1);
	}

	if (token != NULL && strcmp(token, "\\") == 0) {
		/*
		 * Multiple line define
		 */

		while (fgets(line, LINE_SIZE, fp)) {
			len = strlen(line);
			line[len - 1] = '\0';
			find = 0;
			token = strtok(line, " ");

			while (token != NULL) {
				strncat(auxiliar_space, token, LINE_SIZE);
				strncat(auxiliar_space, " ", 1);
				if (strstr(token, "\\"))
					removeChar(auxiliar_space, '\\');
				token = strtok(NULL, " ");

				if (token != NULL && strstr(token, "\\"))
					find = 1;
			}

			if (!find) {
				len = strlen(auxiliar_space) - 1;
				auxiliar_space[len] = '\0';
				break;
			}
		}
	}
	strncat(value, auxiliar_space, LINE_SIZE);
	put(obj, key, value);
}

/*
 * @params
 * obj - the hashmap
 * line - the string in which it will be replaced the strings
 * from the define directive
 * buffer - all the text from the input file untill this moment
 *
 * @value
 * The value of the function is:
 * At the final of the function all the key from the hashmap
 * that will match the word in the stirng it will be replaced
 * with the values in this string
 */
void put_values_text(MyHashMap *obj, char *line, char *buffer)
{
	int i = 0;
	int j = 0;
	int len = 0;
	int counter = 0;
	char word[WORD_SIZE] = {0};
	char *ans = NULL;

	line[strlen(line)] = '\0';
	len = strlen(line);

	if (line[0] == ' ' || line[0] == '\t')
		strncat(buffer, " ", 2);

	for (i = 0; i < len; i++) {
		if (line[i] == '"') {
			word[counter++] = line[i];
			for (j = i + 1; j < len; j++) {
				word[counter++] = line[j];
				if (line[j] == '"')
					break;

			}
			word[counter] = '\0';
			removeChar(word, '\t');
			strncat(buffer, word, BUFFER_SIZE);
			strncpy(word, "", WORD_SIZE);
			counter = 0;
			i = j;
		} else if (!isalpha(line[i])) {
			j = i;
			while (!isalpha(line[j]) && line[j] != '"' && j < len) {
				word[counter++] = line[j];
				j++;
			}
			word[counter] = '\0';
			removeChar(word, '\t');
			strncat(buffer, word, BUFFER_SIZE);
			strncpy(word, "", WORD_SIZE);
			counter = 0;
			i = j - 1;
		} else if (isalpha(line[i])) {
			j = i;
			while ((isalpha(line[j]) || isdigit(line[j])
					|| line[j] == '_') && j < len)
				word[counter++] = line[j++];

			word[counter] = '\0';
			ans = (char *)get(obj, word);
			if (strncmp(word, "input", 5) != 0
				&& strncmp(word, "output", 6) != 0
				&& ans)
				strncpy(word, ans, WORD_SIZE);

			if (strncmp(word, "input", 5) == 0)
				strncpy(word, "input", WORD_SIZE);

			if (strncmp(word, "output", 6) == 0)
				strncpy(word, "output", WORD_SIZE);

			removeChar(word, '\t');
			strncat(buffer, word, BUFFER_SIZE);
			strncpy(word, "", WORD_SIZE);
			counter = 0;
			i = j - 1;
		}
	}
}

/*
 * @params
 * text - the current line of input
 * obj - the hashmap
 * fp - the file descriptor
 * buffer - all the text from the input file untill this moment
 *
 * @values
 * The value of this function is:
 * Depends on the line it will call the define, undef or replace
 * function
 */
void help_ifdef(char *text, MyHashMap *obj, FILE *fp, char *buffer)
{
	if (strncmp(text, "#define", 7) == 0) {
		text[strlen(text) - 1] = '\0';
		resolve_define(text + 8, obj, fp);
	} else if (strncmp(text, "#undef", 6) == 0) {
		removeVal(obj, text + 7);
	} else {
		if (strcmp(text, "\n") != 0 &&
			strncmp(text, "#endif", 6) != 0)
			put_values_text(obj, text, buffer);
	}
}

/*
 * @params
 * text - the current line of input
 * obj - the hashmap
 * fp - file descriptor of the input file
 * buffer - all the text from the input file untill this moment
 * type - is one of the params which will say what function will
 * call : if, ifdef, ifndef
 *
 * @value
 * The value of this function is:
 * It will evaluate all the line between the if, ifndef, ifdef
 * and it will evaluate also the else or elif lines
 */
void resolve_ifdef_ifndef(char *text, MyHashMap *obj, FILE *fp,
						char *buffer, int type)
{
	char *answer = NULL;

	answer = (char *)get(obj, text);
	/*
	 * IFDEF
	 * The clause is true and we need to evaluate the next line
	 */

	if (type == 1 && answer != NULL) {
		fgets(text, LINE_SIZE, fp);
		help_ifdef(text, obj, fp, buffer);

		while (1) {
			fgets(text, LINE_SIZE, fp);
			text[strlen(text) - 1] = '\0';

			if (strncmp(text, "#endif", 6) == 0)
				return;

			if (strncmp(text, "#else", 5) == 0) {
				while (strncmp(text, "#endif", 6) != 0) {
					fgets(text, LINE_SIZE, fp);
					text[strlen(text) - 1] = '\0';
				}
				return;
			}
			strncat(text, "\n", LINE_SIZE);
			help_ifdef(text, obj, fp, buffer);
		}
	}
	/*
	 * IFNDEF
	 * The clause is true and we need to evaluate the next line
	 */

	if (type == 0 && answer == NULL) {
		fgets(text, LINE_SIZE, fp);
		help_ifdef(text, obj, fp, buffer);

		while (1) {
			fgets(text, LINE_SIZE, fp);

			if (strcmp(text, "\n") == 0)
				return;

			text[strlen(text) - 1] = '\0';

			if (strncmp(text, "#endif", 6) == 0)
				return;

			if (strncmp(text, "#else", 5) == 0) {
				while (strncmp(text, "#endif", 6) != 0) {
					fgets(text, LINE_SIZE, fp);
					text[strlen(text) - 1] = '\0';
				}
				return;
			}
			strncat(text, "\n", LINE_SIZE);
			help_ifdef(text, obj, fp, buffer);
		}
	}
	/*
	 * IF
	 * verifi the clause and after that evaluate
	 */

	if ((type == 3 && (strncmp(text, "1", 1) == 0 ||
		strncmp(text, "TRUE", 4) == 0)) ||
		(type == 3 && answer != NULL &&
		((strncmp(answer, "0", 1) != 0) ||
		(strncmp(answer, "TRUE", 4) == 0)))) {

		fgets(text, LINE_SIZE, fp);
		help_ifdef(text, obj, fp, buffer);

		while (1) {
			fgets(text, LINE_SIZE, fp);
			text[strlen(text) - 1] = '\0';

			if (strncmp(text, "#endif", 6) == 0)
				return;

			if (strncmp(text, "#else", 5) == 0) {
				while (strncmp(text, "#endif", 6) != 0) {
					fgets(text, LINE_SIZE, fp);
					text[strlen(text) - 1] = '\0';
				}
				return;
			}

			strncat(text, "\n", LINE_SIZE);
			help_ifdef(text, obj, fp, buffer);
		}
	}
	/*
	 * Push onto the stack the next clause
	 */

	if (strncmp(text, "#elif", 5) == 0) {
		text += 6;
		resolve_ifdef_ifndef(text, obj, fp, buffer, type);
		return;
	}
	/*
	 * Neither of the clauses were true so we have the last else
	 */

	if (strncmp(text, "#else", 5) == 0) {
		fgets(text, LINE_SIZE, fp);
		help_ifdef(text, obj, fp, buffer);

		fgets(text, LINE_SIZE, fp);
		text[strlen(text) - 1] = '\0';
		resolve_ifdef_ifndef(text, obj, fp, buffer, type);
	}
	/*
	 * Final of statement
	 */
	if (strncmp(text, "#endif", 6) == 0)
		return;

	fgets(text, LINE_SIZE, fp);
	text[strlen(text) - 1] = '\0';
	resolve_ifdef_ifndef(text, obj, fp, buffer, 3);
}

/*
 * @params
 * line - the current line of the input
 * buffer - all the text from the input file untill this moment
 * fp - the file descriptor for the current input file
 * input_file - the name of the #include directory
 * directories - all the files from the -I directive
 *
 * @value
 * The value of this function is:
 * At the final of this function we will have all the text from
 * the name of the #include directive, replaced in original file
 * and also it will be replaced with all the #define directive values
 * and also will evaluate all the if, ifdef, ifndef directives
 */
int resolve_include(char *line, MyHashMap *obj, char *buffer, FILE *fp,
			char *input_file,
			char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY])
{
	FILE *include_file;
	char *token, path[LINE_SIZE], buffer_copy[BUFFER_SIZE];
	int i, len1, len2;

	i = 0;
	len1 = 0;
	len2 = 0;
	include_file = NULL;

	memset(path, 0, LINE_SIZE);
	token = strtok(line, "\"");
	strcpy(line, "");
	memcpy(line, token, strlen(token));
	line[strlen(line) - 1] = '\0';

	/*
	 * Open the file
	 */

	strcat(path, line);
	include_file = fopen(path, "r");
	if (include_file == NULL) {
		/*
		 * the file is not in the current folder
		 */
		token = strrchr(input_file, '/');
		len1 = strlen(input_file);
		len2 = strlen(token);
		strcpy(path, "");
		for (i = 0; i <= len1 - len2; i++)
			path[i] = input_file[i];

		path[i] = '\0';
		strcat(path, line);
		include_file = fopen(path, "r");

		if (include_file == NULL) {
			/*
			 * the file is not in the input's folder
			 */
			for (i = 0; i < size_directory; i++) {
				strcpy(path, directories[i]);
				strcat(path, "/");
				strcat(path, line);
				include_file = fopen(path, "r");
				if (include_file != NULL)
					break;
			}
		}
	}
	if (include_file != NULL) {
		memset(buffer_copy, 0, BUFFER_SIZE);
		if (read_from_file(include_file, obj, buffer_copy,
						path, directories) == 0) {
			fclose(include_file);
			return 0;
		}
		strcpy(buffer, buffer_copy);
		fclose(include_file);
	} else {
		return 0;
	}
	return 1;
}

/*
 * @params
 * fp - the current fiel descriptor from input file
 * obj - the hashmap
 * buffer - all the text from the input file untill this moment
 * input_file - the name of the input file
 * directories - all the directories from the -I directive
 *
 * @value
 * The value of tthis function is:
 * This functions takes line by line and evaluate each line
 * Depends of the line it will call the specific function
 */
int read_from_file(FILE *fp, MyHashMap *obj, char *buffer,
			char *input_file,
			char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY])
{
	char line[LINE_SIZE];

	memset(line, 0, LINE_SIZE);
	while (fgets(line, LINE_SIZE, fp)) {
		if (strncmp(line, "#ifdef", 6) == 0) {
			/*
			 * Handle ifdef, put at the end of the file \0
			 * and skip the #ifdef string
			 */
			line[strlen(line) - 1] = '\0';
			resolve_ifdef_ifndef(line + 7, obj, fp, buffer, 1);
		} else if (strncmp(line, "#ifndef", 7) == 0) {
			/*
			 * Handle ifndef
			 */
			line[strlen(line) - 1] = '\0';
			resolve_ifdef_ifndef(line + 8, obj, fp, buffer, 0);
		} else if (strncmp(line, "#include", 8) == 0) {
			/*
			 * Handle include
			 */
			if (resolve_include(line + 9, obj, buffer, fp,
						input_file, directories) == 0)
				return 0;
		} else if (strncmp(line, "#if", 3) == 0) {
			/*
			 * Handle if
			 */
			line[strlen(line) - 1] = '\0';
			resolve_ifdef_ifndef(line + 4, obj, fp, buffer, 3);
		} else if (strncmp(line, "#undef", 6) == 0) {
			/*
			 * Handle undef
			 */
			removeVal(obj, line + 7);
		} else if (strncmp(line, "#define", 7) == 0) {
			/*
			 * Handle define
			 */
			line[strlen(line) - 1] = '\0';
			resolve_define(line + 8, obj, fp);
		} else {
			if (strcmp(line, "\n") != 0 &&
				strncmp(line, "#endif", 6) != 0)
				put_values_text(obj, line, buffer);
		}
	}
	return 1;
}

/*
 * @params
 * input_file - the name of the input file
 * obj - the hashmap
 * directories - the files from the -I directives
 *
 * @values
 * The value of this function is:
 * This function opens the input file
 * This function calls the read_from_file function
 * This function return the text replaced and evaluated
 */
int open_input(char *input_file, MyHashMap *obj,
			char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY])
{
	FILE *fp = NULL;
	FILE *fo = NULL;
	char buffer[BUFFER_SIZE] = {0};
	char *ans = NULL;

	fp = fopen(input_file, "r");
	memset(buffer, 0, BUFFER_SIZE);

	if (fp == NULL)
		return 0;

	if (read_from_file(fp, obj, buffer, input_file, directories) == 0) {
		fclose(fp);
		return 0;
	}

	strncat(buffer, "\n", 1);
	ans = (char *)get(obj, "output");
	if (ans) {
		fo = fopen(ans, "w");
		if (fo == NULL) {
			fclose(fp);
			return 0;
		}
		strncat(buffer, "\n", 1);
		fprintf(fo, "%s", buffer);
		fclose(fo);
	} else {
		printf("%s", buffer);
	}
	fclose(fp);
	return 1;
}
