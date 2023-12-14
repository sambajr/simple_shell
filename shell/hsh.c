#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char *find_in_path(char *command) {
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *p = strtok(path_copy, ":");
    char *fullpath = malloc(1024);

    while (p != NULL) {
        sprintf(fullpath, "%s/%s", p, command);

        // Use stat instead of access
        struct stat st;
        if (stat(fullpath, &st) == 0 && st.st_mode & S_IXUSR) {
            free(path_copy);
            return fullpath;
        }

        p = strtok(NULL, ":");
    }

    free(path_copy);
    free(fullpath);
    return NULL;
}

int main() {
    char *result = find_in_path("ls");
    if (result != NULL) {
        printf("Command found at: %s\n", result);

        pid_t pid = fork();

        if (pid == -1) {
            perror("Error in fork:");
        } else if (pid == 0) {
            // Child process
            if (execve(result, (char *[]) {result, NULL}, NULL) == -1) {
                perror("Error in execve:");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("Child process exited with status %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Child process terminated by signal %d\n", WTERMSIG(status));
            }
        }
    } else {
        printf("Command not found\n");
    }

    return 0;
}
