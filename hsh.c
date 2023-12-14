#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h> /* Include string.h for strchr function */

#define MAXARGS 64

/* Function to split the command line into arguments */
char **split_line(char *line) {
    char **args = malloc(MAXARGS * sizeof(char *));
    char *arg;
    int i = 0;

    arg = line;
    while (arg != NULL) {
        args[i] = arg;
        arg = strchr(arg, ' ');
        if (arg != NULL) {
            *arg = '\0';
            arg++;
        }
        i++;
    }
    args[i] = NULL;
    return args;
}

int main() {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char **argv;
    pid_t pid;
    int status;

    while (1) {
        printf("$ ");
        read = getline(&line, &len, stdin);
        if (read == -1) { /* Handling EOF */
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        line[read - 1] = '\0'; /* Remove newline at the end */

        argv = split_line(line);

        if ((pid = fork()) == -1) {
            perror("Error:");
            continue;
        }
        if (pid == 0) {
            if (execve(argv[0], argv, NULL) == -1) {
                perror("Error:");
            }
            exit(EXIT_FAILURE);
        } else {
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        free(argv);
    }
    free(line);
    return EXIT_SUCCESS;
}




