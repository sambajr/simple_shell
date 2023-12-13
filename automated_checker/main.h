#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define MAX_ARGS 64
extern char **environ;
char *get_user_input(void);
void interactive_mode(void);
void non_interactive_mode(const char *script);
void parse_input(char *input, char *args[]);
void check_env_command(char *args[]);
void check_exit_command(char *args[]);
void execute_command(char *args[]);

#endif /* MAIN_H */
