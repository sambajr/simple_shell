/**
 * parse_input - Tokenize user input into command and arguments
 * @input: User input string to be parsed
 * @args: Array to store the parsed arguments
 *
 * This function takes a user input string and tokenizes it into individual
 * words using space as a delimiter. It populates the given array of arguments
 * with these tokens and null-terminates the array to prepare it for execution.
 *
 * @input: User input string to be parsed
 * @args: Array to store the parsed arguments
 */
#include "main.h"
void parse_input(char *input, char *args[])
{
	size_t arg_count = 0;
	char *token = strtok(input, " ");

	while (token != NULL && arg_count < MAX_ARGS - 1)
	{
		args[arg_count++] = token;
		token = strtok(NULL, " ");
	}

	args[arg_count] = NULL;  /*Null-terminate the args array*/
}
