#include "main.h"
/**
 * execute_command - Execute a command using fork and execve
 * @args: Array of arguments for the command
 */
void execute_command(char *args[])
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		char *command = args[0];

		if (command == NULL)
		{
			fprintf(stderr, ":( No command provided\n");
			exit(EXIT_FAILURE);
		}
		if (strncmp(command, "/bin/", 5) != 0)
		{
			char path_command[256];
			snprintf(path_command, sizeof(path_command), "/bin/%s", command);

			if (access(path_command, X_OK) == -1)
			{
				fprintf(stderr, ":( Command not found in PATH\n");
				exit(EXIT_FAILURE);
			}

			if (execve(path_command, args, NULL) == -1)
			{
				perror("ERROR");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (execve(command, args, NULL) == -1)
			{
				perror("ERROR");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		wait(NULL);
	}
}
