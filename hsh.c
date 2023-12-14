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
	char *path_copy = strdup(path);
	char *p;
	char *fullpath = malloc(1024);

    /* If command starts with '/', it's an absolute path */
	if (command == NULL)
	{
		return (NULL);
	}
	if (command[0] == '/')
	{
		if (stat(command, &st) == 0 && st.st_mode & S_IXUSR)
			return (strdup(command));
		else
			return (NULL);
	}

    /* Otherwise, search for the command in the PATH */

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
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	char *command;
	pid_t pid;
	int status;
	int i;
	int line_number = 0;
	int contains_non_whitespace = 0;
	size_t j;

	/*char line_str[12];*/

	(void)argc;


	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);
		read = getline(&line, &len, stdin);
		line_number++;

		if (read == -1)
		{
			if (isatty(STDIN_FILENO))
				fprintf(stderr, "\n");
			free(line);
			exit(EXIT_SUCCESS);
		}

/* Trim leading and trailing whitespaces */
		while (read > 0 && (line[read - 1] == '\n' || line[read - 1] == ' ' || line[read - 1] == '\t'))
		{
			line[--read] = '\0';
		}

/* If the line contains only whitespace characters or is empty, prompt again */

		for (j = 0; j < (size_t)read; j++)
		{
			if (line[j] != ' ' && line[j] != '\t')
			{
				contains_non_whitespace = 1;
				break;
			}
		}

		if (!contains_non_whitespace)
		{
			continue;
		}

		args = split_line(line);

		/* Check exit */
		if (args[0] != NULL && strcmp(args[0], "exit") == 0)
		{
			free(args);
			break;
		}
			/* Check env */
		if (args[0] != NULL && strcmp(args[0], "env") == 0)
		{
			for (i = 0; environ[i]; i++)
			{
				fprintf(stdout, "%s\n", environ[i]);
			}
			free(args);
			continue;
		}
		command = find_in_path(args[0]);

		if (command == NULL)
		{
			fprintf(stderr, "%s: No such file or directory\n", argv[0]);
			free(args);
			continue;
		}
		pid = fork();
		if (pid == 0)
		{
			if (execve(command, args, NULL) == -1)
			{
				/*Error message won't be displayed*/
				exit(EXIT_FAILURE);
			}
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			perror("ERROR:\n");
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
