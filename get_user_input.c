#include "main.h"
#include <stddef.h>

char *get_user_input(void)
{
    char *input = NULL;
    size_t n = 0;

    // Check if stdin is a terminal before prompting for input
    if (isatty(fileno(stdin)))
    {
        printf("GOAT$ ");
        fflush(stdout); // Ensure prompt is printed immediately
    }

    if (getline(&input, &n, stdin) == -1)
    {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    input[strcspn(input, "\n")] = '\0';
    return (input);
}
