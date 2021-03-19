#include "utilities.c"
int main(int argc, char *argv[])
{
	MyHashMap *my_hash_map;
	char *answer, directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY];
	char line[LINE_SIZE];

	my_hash_map = create();
	if (my_hash_map == NULL)
		return ENOMEM;

	memset(line, 0, LINE_SIZE);
	check_params(argc, argv, my_hash_map, directories);

	answer = (char *)get(my_hash_map, "input");
	if (answer) {
		if (open_input(answer, my_hash_map, directories) == 0) {
			free_map(my_hash_map);
			return 1;
		}
	} else {
		fgets(line, LINE_SIZE, stdin);
		line[strlen(line) - 1] = '\0';
		printf("%s\n", line);
	}

	/*
	 * free zone
	 */
	free_map(my_hash_map);
	return 0;
}


