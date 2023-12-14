
// Update the function prototype in interactive_mode.c
#include "main.h"

void interactive_mode(int isatty_flag);

// Modify interactive_mode.c
void interactive_mode(int isatty_flag)
{
    char *input = NULL;
    char *args[MAX_ARGS];

    while (1)
    {
        if (isatty_flag)
        {
            input = get_user_input();
            printf("GOAT$ ");
            fflush(stdout); // Ensure prompt is printed immediately
        }
        else
        {
            input = get_user_input();
        }

        parse_input(input, args);
        check_exit_command(args);
        execute_command(args);
        check_env_command(args);
        free(input);
    }
}
