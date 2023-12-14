// Update the function prototype in main.c
#include "main.h"

int main(int argc, char *argv[])
{
    int isatty_flag = isatty(fileno(stdin));

    if (argc == 1)
    {
        interactive_mode(isatty_flag);
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
