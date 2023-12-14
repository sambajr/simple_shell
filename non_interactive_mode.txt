#include "main.h"

/**
 * non_interactive_mode - Run the shell in non-interactive mode
 * @script: File containing commands to execute
 */
void non_interactive_mode(const char *script)
{
	FILE *file = fopen(script, "r");
	char *line = NULL;
	size_t len = 0;

	if (!file)
	{
		perror("Error opening script file");
		exit(EXIT_FAILURE);
	}

	while (getline(&line, &len, file) != -1)
	{
		char *args[MAX_ARGS];

		parse_input(line, args);
		execute_command(args);
	}
	fclose(file);
	free(line);
}
