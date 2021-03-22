#include "utilities.c"
int main(int argc, char *argv[])
{
	/*
	 *	Initialize the map and the structures for directories
	 */
	MyHashMap *my_hash_map = NULL;
	char *answer = NULL;
	char directories[NR_DIRECTORIES][LENGTH_NAME_DIRECTORY] = {0};
	char line[LINE_SIZE] = {0};
	size_directory = 0;
	/*
	 *	Create the map and verify if it fails
	 */
	my_hash_map = create();
	if (my_hash_map == NULL)
		exit(ENOMEM);

	memset(line, 0, LINE_SIZE);

	/*
	 * Get the params from the command line
	 */
	check_params(argc, argv, my_hash_map, directories);

	answer = (char *)get(my_hash_map, "input");
	if (answer) {
		if (open_input(answer, my_hash_map, directories) == 0) {
			free_map(my_hash_map);
			return 2;
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


