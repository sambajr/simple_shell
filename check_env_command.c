#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * check_env_command - Check and handle the env command
 * @args: Array of arguments to check
 *
 * This function checks if the first argument is "env" and prints the
 * current environment if true.
 */

void check_env_command(char *args[])
{
	if (args[0] != NULL && strcmp(args[0], "env") == 0)
	{
		unsigned int i = 0;

		while (environ[i] != NULL)
		{
			printf("%s\n", environ[i]);
			i++;
		}
	}
}
