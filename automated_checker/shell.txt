// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

#define MAX_ARGS 64

void parse_input(char *input, char *args[]);
void execute_command(char *args[]);
void check_exit_command(char *args[]);
void check_env_command(char *args[]);

void free_args(char *args[])
{
    for (size_t i = 0; args[i] != NULL; ++i)
    {
        free(args[i]);
    }
}

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
    return input;
}

void parse_input(char *input, char *args[])
{
    if (input == NULL)
    {
        fprintf(stderr, "Error: NULL input\n");
        return;
    }
    size_t arg_count = 0;
    char *token = strtok(input, " \t\n"); // Include newline as a delimiter

    while (token != NULL && arg_count < MAX_ARGS - 1)
    {
        args[arg_count++] = strdup(token); // Use strdup to copy the token

        token = strtok(NULL, " \t\n");
    }

    args[arg_count] = NULL; // Null-terminate the args array
}

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

void check_exit_command(char *args[])
{
    if (args[0] != NULL && strcmp(args[0], "exit") == 0)
    {
        if (args[1] != NULL)
        {
            int exit_code = atoi(args[1]);
            free(args[1]);
            exit(exit_code);
        }
        free(args[0]);
        exit(0);
    }
}
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
        // Child process code

        // Construct the full path to the executable
        char *command = args[0];
        char path_command[256];

        if (strncmp(command, "/bin/", 5) != 0)
        {
            snprintf(path_command, sizeof(path_command), "/bin/%s", command);
            args[0] = path_command;
        }

        // Execute the command and handle errors
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execve failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Parent process code
        int status;
        waitpid(pid, &status, 0);

        // Check if the child process exited successfully
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) != 0)
            {
                fprintf(stderr, "Command failed to execute with exit status %d\n", WEXITSTATUS(status));
            }
        }
        else if (WIFSIGNALED(status))
        {
            fprintf(stderr, "Command terminated by signal %d\n", WTERMSIG(status));
        }
        else
        {
            fprintf(stderr, "Command did not exit successfully\n");
        }
    }
}

void non_interactive_mode(const char *script)
{
    FILE *file = fopen(script, "r");
    char *line = NULL;
    size_t len = 0;

    if (!file)
    {
        perror("Error opening script file");
        return;
    }

    while (getline(&line, &len, file) != -1)
    {
        char *args[MAX_ARGS];
        parse_input(line, args);
        execute_command(args);

        if (getline(&line, &len, file) != -1)
        {
            printf("GOAT$ ");
        }
    }

    fclose(file);
    free(line);
}

void interactive_mode(void)
{
    char *input = NULL;
    char *args[MAX_ARGS];

    while (1)
    {
        input = get_user_input();
        parse_input(input, args);
        check_exit_command(args);
        execute_command(args);
        check_env_command(args);
        free(input);
    }
}


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

    return 0;
}
