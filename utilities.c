#include "utilities.h"

void param_D(int number_args, char **arguments, MyHashMap *obj, char *arg,
		int len, int *i)
{
	int new_pos = 0, pos = 0, val_pos = 0;
	char name_define[256], val_define[256];

	strcpy(name_define, "");
	strcpy(val_define, "");
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
	if (strcmp(val_define, "") == 0)
		put(obj, name_define, "");
	else
		put(obj, name_define, val_define);

	strcpy(name_define, "");
	strcpy(val_define, "");
}

void param_O(int number_args, char **arguments, MyHashMap *obj, char *arg,
			int len, int *i)
{
	int new_pos;
	char value[LINE_SIZE];

	new_pos = 0;
	memset(value, 0, LINE_SIZE);

	if (len == 2) {
		strcpy(value, arguments[++*i]);
	} else {
		for (new_pos = 2; new_pos < len; new_pos++)
			value[new_pos - 2] = arg[new_pos];
		value[new_pos++] = '\0';
	}
	put(obj, "output", value);
}

void param_I(int number_args, char **arguments, MyHashMap *obj, char *arg,
		int len, int *i,
		char directory[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY])
{
	int new_pos = 0, pos = 0;
	char name_define[256], val_define[256];
	static int l;

	strcpy(name_define, "");
	strcpy(val_define, "");

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
	strcpy(directory[l++], val_define);
	size_directory = l;
	strcpy(val_define, "");
}

void check_params(int number_args, char **arguments,
			MyHashMap *obj,
			char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY])
{
	int len = 0;
	char *arg = NULL;

	for (int i = 1; i < number_args; i++) {
		arg = arguments[i];
		len = strlen(arg);
		if (strncmp(arg, "-D", 2) == 0) {
			param_D(number_args, arguments, obj, arg, len, &i);
		} else if (strncmp(arg, "-I", 2) == 0) {
			param_I(number_args, arguments, obj,
					arg, len, &i, directories);
		} else if (strncmp(arg, "-o", 2) == 0) {
			param_O(number_args, arguments, obj, arg, len, &i);
		} else {
			if (strstr(arg, ".out"))
				put(obj, "output", arg);
			else
				put(obj, "input", arg);
		}
	}
}

void removeChar(char *str, char garbage)
{
	char *src, *dst;

	for (src = dst = str; *src != '\0'; src++) {
		*dst = *src;
		if (*dst != garbage)
			dst++;
	}
	*dst = '\0';
}

void resolve_define(char *text, MyHashMap *obj, FILE *fp)
{
	char *token, *key, *ans;
	char auxiliar_space[LINE_SIZE], line[LINE_SIZE], value[LINE_SIZE];
	int len, find = 0;

	memset(auxiliar_space, 0, LINE_SIZE);
	memset(line, 0, LINE_SIZE);
	memset(value, 0, LINE_SIZE);
	token = strtok(text, " ");
	key = token;
	token = strtok(NULL, " ");

	while (token != NULL && strcmp(token, "\\") != 0) {
		ans = (char *)get(obj, token);
		find = 0;
		while (ans) {
			if (ans) {
				strcat(auxiliar_space, ans);
				ans = (char *)get(obj, ans);
				find = 1;
			} else {
				break;
			}
		}
		if (!find)
			strcat(auxiliar_space, token);

		token = strtok(NULL, " ");

		if (token != NULL)
			strcat(auxiliar_space, " ");
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
				strcat(auxiliar_space, token);
				strcat(auxiliar_space, " ");
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
	strcat(value, auxiliar_space);
	put(obj, key, value);
}

void put_values_text(MyHashMap *obj, char *line, char *buffer)
{
	int i, j, len, counter;
	char word[WORD_SIZE], *ans;

	i = 0;
	j = 0;
	line[strlen(line)] = '\0';
	len = strlen(line);
	counter = 0;
	memset(word, 0, WORD_SIZE);

	if (line[0] == ' ' || line[0] == '\t')
		strcat(buffer, " ");

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
			strcat(buffer, word);
			strcpy(word, "");
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
			strcat(buffer, word);
			strcpy(word, "");
			counter = 0;
			i = j - 1;
		} else if (isalpha(line[i])) {
			j = i;
			while ((isalpha(line[j]) || isdigit(line[j])
					|| line[j] == '_') && j < len)
				word[counter++] = line[j++];

			word[counter] = '\0';
			ans = (char *)get(obj, word);
			if (strcmp(word, "input") != 0
				&& strcmp(word, "output") != 0
				&& ans)
				strcpy(word, ans);

			if (strcmp(word, "input") == 0)
				strcpy(word, "input");

			if (strcmp(word, "output") == 0)
				strcpy(word, "output");

			removeChar(word, '\t');
			strcat(buffer, word);
			strcpy(word, "");
			counter = 0;
			i = j - 1;
		}
	}
}

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

void resolve_ifdef_ifndef(char *text, MyHashMap *obj, FILE *fp,
						char *buffer, int type)
{
	char *answer;

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

			if (strcmp(text, "#endif") == 0)
				return;

			if (strcmp(text, "#else") == 0) {
				while (strcmp(text, "#endif") != 0) {
					fgets(text, LINE_SIZE, fp);
					text[strlen(text) - 1] = '\0';
				}
				return;
			}
			strcat(text, "\n");
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

			if (strcmp(text, "#endif") == 0)
				return;

			if (strcmp(text, "#else") == 0) {
				while (strcmp(text, "#endif") != 0) {
					fgets(text, LINE_SIZE, fp);
					text[strlen(text) - 1] = '\0';
				}
				return;
			}
			strcat(text, "\n");
			help_ifdef(text, obj, fp, buffer);
		}
	}
	/*
	 * IF
	 * verifi the clause and after that evaluate
	 */

	if ((type == 3 && (strcmp(text, "1") == 0 ||
		strcmp(text, "TRUE") == 0)) ||
		(type == 3 && answer != NULL &&
		((strcmp(answer, "0") != 0) ||
		(strcmp(answer, "TRUE") == 0)))) {

		fgets(text, LINE_SIZE, fp);
		help_ifdef(text, obj, fp, buffer);

		while (1) {
			fgets(text, LINE_SIZE, fp);
			text[strlen(text) - 1] = '\0';

			if (strcmp(text, "#endif") == 0)
				return;

			if (strcmp(text, "#else") == 0) {
				while (strcmp(text, "#endif") != 0) {
					fgets(text, LINE_SIZE, fp);
					text[strlen(text) - 1] = '\0';
				}
				return;
			}

			strcat(text, "\n");
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

	if (strcmp(text, "#else") == 0) {
		fgets(text, LINE_SIZE, fp);
		help_ifdef(text, obj, fp, buffer);

		fgets(text, LINE_SIZE, fp);
		text[strlen(text) - 1] = '\0';
		resolve_ifdef_ifndef(text, obj, fp, buffer, type);
	}
	/*
	 * Final of statement
	 */
	if (strcmp(text, "#endif") == 0)
		return;

	fgets(text, LINE_SIZE, fp);
	text[strlen(text) - 1] = '\0';
	resolve_ifdef_ifndef(text, obj, fp, buffer, 3);
	return;
}

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

int open_input(char *input_file, MyHashMap *obj,
			char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY])
{
	FILE *fp = NULL;
	FILE *fo = NULL;
	char buffer[BUFFER_SIZE], *ans;

	fp = fopen(input_file, "r");
	memset(buffer, 0, BUFFER_SIZE);

	if (fp == NULL)
		return 0;

	if (read_from_file(fp, obj, buffer, input_file, directories) == 0) {
		fclose(fp);
		return 0;
	}

	strcat(buffer, "\n");
	ans = (char *)get(obj, "output");
	if (ans) {
		fo = fopen(ans, "w");
		if (fo == NULL) {
			fclose(fp);
			return 0;
		}
		strcat(buffer, "\n");
		fprintf(fo, "%s", buffer);
		fclose(fo);
	} else {
		printf("%s", buffer);
	}
	fclose(fp);
	return 1;
}