#include "main.h"

/**
 * main - Entry point for the GOAT Shell program
 * @argc: Number of arguments
 * @argv: Array of command-line arguments
 *
 * This function determines whether to enter interactive or non-interactive
 * mode based on the number of command-line arguments. If no arguments are
 * provided, it enters interactive mode. If one argument is provided, it
 * enters non-interactive mode with the specified script. Otherwise, it
 * prints a usage message and exits with failure.
 *
 * Return: Always returns 0.
 */
int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		interactive_mode();
	}
	else if (argc == 2)
	{
		non_interactive_mode(argv[1]);
	}
	else
	{
		fprintf(stderr, "Usage: %s [script]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	return (0);
}
