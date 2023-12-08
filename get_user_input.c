/**
 * get_user_input - Prompt user for input and read from stdin
 *
 * This function displays the prompt "GOAT$" to the user, reads input from
 * stdin using the getline function, and returns the entered string. It removes
 * the newline character from the end of the input to provide a clean string.
 *
 * Return: A dynamically allocated string containing the user input.
 */
#include "main.h"
char *get_user_input(void)
{
	char *input = NULL;
	size_t n = 0;

	printf("GOAT$ ");
	if (getline(&input, &n, stdin) == -1)
	{
		perror("ERROR");
		exit(EXIT_FAILURE);
	}

	input[strcspn(input, "\n")] = '\0';
	return (input);
}
