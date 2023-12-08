#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/**
 * check_exit_command - Check and handle the exit command
 * @args: Array of arguments to check
 *
 * This function checks if the first argument is "exit" and handles
 * the exit command accordingly. If additional arguments are provided,
 * it converts the second argument to an integer and exits with that code.
 * If no additional arguments are provided, it prints a farewell message
 * and exits with code 0.
 */
void check_exit_command(char *args[])
{
	if (args[0] != NULL && strcmp(args[0], "exit") == 0)
	{
		if (args[1] != NULL)
		{
			int exit_code = atoi(args[1]);

			free(args[0]);
			exit(exit_code);
		}
		else
		{
			free(args[0]);
			printf("BYE USER !!\n");
			exit(0);
		}
	}
}
