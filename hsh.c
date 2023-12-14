#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "main.h"


/**
 * find_in_path - find command in PATH
 * @command: command to find
 *
 * Return: full path to command or NULL if not found
 */
char *find_in_path(char *command)
{
	struct stat st;
	char *path = getenv("PATH");
	char *path_copy;
	char *p;
	char *fullpath = malloc(1024);

    /* If command starts with '/', it's an absolute path */
	if (command[0] == '/')
	{
		if (stat(command, &st) == 0 && st.st_mode & S_IXUSR)
			return (strdup(command));
		else
			return (NULL);
	}

    /* Otherwise, search for the command in the PATH */
	path_copy = strdup(path);
	p = strtok(path_copy, ":");

	while (p != NULL)
	{
		sprintf(fullpath, "%s/%s", p, command);
		if (stat(fullpath, &st) == 0 && st.st_mode & S_IXUSR)
		{
			free(path_copy);
			return (fullpath);
		}
		p = strtok(NULL, ":");
	}

	free(path_copy);
	free(fullpath);
	return (NULL);
}

/**
 * split_line - split line into tokens
 * @line: line to split
 *
 * Return: array of tokens
 */
char **split_line(char *line)
{
	char **tokens = malloc(64 * sizeof(char *));
	char *token;
	int position = 0;

	token = strtok(line, " ");
	while (token != NULL)
	{
		tokens[position] = token;
		position++;
		token = strtok(NULL, " ");
	}
	tokens[position] = NULL;
	return (tokens);
}

/**
 * main - main function for the shell
 *
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	char *command;
	pid_t pid;
	int status;
	int i;

	while (1)
	{
		write(1, "$ ", 2);
		read = getline(&line, &len, stdin);

		if (read == -1)
		{
			write(1, "\n", 1);
			free(line);
			exit(EXIT_SUCCESS);
		}

		line[read - 1] = '\0';
		args = split_line(line);
		/* Check exit */
		if (strcmp(args[0], "exit") == 0)
		{
			free(args);
			free(line);
			exit(EXIT_SUCCESS);
		}
		/* Check env */
		if (strcmp(args[0], "env") == 0)
		{
			for (i = 0; environ[i]; i++)
			{
				write(1, environ[i], strlen(environ[i]));
				write(1, "\n", 1);
			}
			free(args);
			continue;
		}

		command = find_in_path(args[0]);

		if (command == NULL)
		{
			write(2, "Error: Command '", 16);
			write(2, args[0], strlen(args[0]));
			write(2, "' not found in PATH\n", 19);
			free(args);
			continue;
		}

		pid = fork();
		if (pid == 0)
		{
			if (execve(command, args, NULL) == -1)
			{
				perror("Error:");
			}
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			perror("Error:");
		}
		else
		{
			do {
				waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
		free(args);
		free(command);
	}
	free(line);
	return (0);
}
