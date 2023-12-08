#include "main.h"

/**
 * interactive_mode - Run the shell in interactive mode
 */
void interactive_mode(void)
{
	/*char *command;*/
	char *input = NULL;

	char *args[MAX_ARGS];

	system("cowsay -f tux \"Welcome to GOAT SHELL\"");

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
