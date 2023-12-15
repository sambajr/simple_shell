#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "main.h"


void set_env_variable(char *variable, char *value);


/**
 * find_in_path - Find command in PATH
 * @command: Command to find
 *
 * Return: Full path to command or NULL if not found
 */
char *find_in_path(char *command)
{
    struct stat st;
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *p;
    char *fullpath = malloc(1024);

    if (command == NULL)
        return (NULL);

    if (command[0] == '/')
    {
        if (stat(command, &st) == 0 && st.st_mode & S_IXUSR)
            return (strdup(command));
        else
            return (NULL);
    }

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
 * split_line - Split line into tokens
 * @line: Line to split
 *
 * Return: Array of tokens
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
 * set_env_variable - set or modify environment variable
 * @variable: environment variable name
 * @value: environment variable value
 */
void set_env_variable(char *variable, char *value)
{
    if (variable == NULL || value == NULL)
    {
        fprintf(stderr, "Usage: setenv <variable> <value>\n");
        return;
    }

    if (setenv(variable, value, 1) == -1)
    {
        perror("Error setting environment variable");
    }
}

/**
 * unset_env_variable - unset environment variable
 * @variable: environment variable name
 */
void unset_env_variable(char *variable)
{
    if (variable == NULL)
    {
        fprintf(stderr, "Usage: unsetenv <variable>\n");
        return;
    }

    if (unsetenv(variable) == -1)
    {
        perror("Error unsetting environment variable");
    }
}
/**
 * cd_command - Change the current directory of the process.
 * @args: Array of arguments (command and optional directory).
 * @home: Home directory for the user.
 *
 * Return: 0 on success, -1 on failure.
 */
int cd_command(char **args, char *home)
{
    char *directory;
	char cwd[1024];
	
	
    if (args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        directory = home;
    }
    else if (strcmp(args[1], "-") == 0)
    {
        directory = getenv("OLDPWD");
        if (directory == NULL)
        {
            fprintf(stderr, "OLDPWD not set\n");
            return -1;
        }
        printf("%s\n", directory);
    }
    else
    {
        directory = args[1];
    }

    if (chdir(directory) != 0)
    {
        perror("cd");
        return -1;
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        set_env_variable("OLDPWD", getenv("PWD"));
        set_env_variable("PWD", cwd);
    }
    else
    {
        perror("getcwd");
        return -1;
    }

    return 0;
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
    pid_t pid;
    int status;
    int line_number = 0, i;
    int contains_non_whitespace = 0;
    size_t j;
	char *home_directory;

    (void)argc;
	
	home_directory = getenv("HOME");

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
            if (args[1] != NULL)
            {
                int exit_status = atoi(args[1]);
                free(args);
                free(line);
                exit(exit_status);
            }
            else
            {
                free(args);
                free(line);
                exit(EXIT_SUCCESS);
            }
        }

        /* Check env */
        if (args[0] != NULL && strcmp(args[0], "env") == 0)
        {
            for ( i = 0; environ[i]; i++)
            {
                fprintf(stdout, "%s\n", environ[i]);
            }
            free(args);
            continue;
        }

        /* Check setenv */
        if (args[0] != NULL && strcmp(args[0], "setenv") == 0)
        {
            if (args[1] != NULL && args[2] != NULL)
            {
                set_env_variable(args[1], args[2]);
            }
            free(args);
            continue;
        }

        /* Check unsetenv */
        if (args[0] != NULL && strcmp(args[0], "unsetenv") == 0)
        {
            if (args[1] != NULL)
            {
                unset_env_variable(args[1]);
            }
            free(args);
            continue;
        }

		/* Check cd */
        if (args[0] != NULL && strcmp(args[0], "cd") == 0)
        {
            if (cd_command(args, home_directory) == -1)
            {
                fprintf(stderr, "cd: Unable to change directory\n");
            }
            free(args);
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            /* Child process */
            char *command_path = find_in_path(args[0]);

            if (command_path == NULL)
            {
                fprintf(stderr, "%s: command not found\n", argv[0]);
                free(args);
                exit(EXIT_FAILURE);
            }

            if (execve(command_path, args, environ) == -1)
            {
                perror("execve");
                free(args);
                free(command_path);
                exit(EXIT_FAILURE);
            }

            free(command_path);
            free(args);
            exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
        {
            perror("fork");
        }
        else
        {
            /* Parent process */
            do
            {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        free(args);
    }

    free(line);
    return 0;
}
