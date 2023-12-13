#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10  /*Adjust the maximum number of arguments as needed*/

/* Function to execute a command*/
void execute_command(char *args[]);
extern char **environ;
/*Function to process user input*/
char *get_user_input(void);

/* Function to parse user input into arguments*/
void parse_input(char *input, char *args[]);

/*Function to check and handle the exit command*/
void check_exit_command(char *args[]);
void interactive_mode(int isatty_flag);
void non_interactive_mode(const char *script);
void check_env_command(char *args[]);


#endif /* MAIN_H */
