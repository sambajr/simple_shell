#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"
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

        // Execute the command and handle errors
        execvp(args[0], args);

        // If execvp fails, print an error message and exit
        perror("Command execution failed");
        exit(EXIT_FAILURE);
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


