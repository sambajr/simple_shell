GOAT Shell
Welcome to GOAT Shell, a simple and interactive shell program that supports basic commands and scripting.

Table of Contents:
>Overview
>Files
>Usage
>Features
>Contributing
Overview
GOAT Shell is a lightweight shell program written in C that provides a simple command-line interface for users. It supports interactive and non-interactive modes, allowing you to run commands in real-time or execute scripts.
Files
The GOAT Shell consists of several modular files, each serving a specific purpose:

check_env_command.c: Handles the env command to display the current environment.
check_exit_command.c: Manages the exit command, allowing the user to exit the shell with an optional status code.
execute_command.c: Executes a given command using fork and execve.
get_user_input.c: Prompts the user for input and reads from standard input.
interactive_mode.c: Implements the interactive mode of the shell.
main.c: The main entry point of the GOAT Shell program, determining whether to run in interactive or non-interactive mode.
non_interactive_mode.c: Implements the non-interactive mode of the shell, executing commands from a script.
parse_input.c: Tokenizes user input into commands and arguments.
main.h: Header file containing function declarations and necessary includes.


Usage
To run the GOAT Shell interactively, simply execute the binary:
./goat_shell

GOAT Shell
Welcome to GOAT Shell, a simple and interactive shell program that supports basic commands and scripting.

Table of Contents
Overview
Files
Usage
Features
Contributing
License
Overview
GOAT Shell is a lightweight shell program written in C that provides a simple command-line interface for users. It supports interactive and non-interactive modes, allowing you to run commands in real-time or execute scripts.

Files
The GOAT Shell consists of several modular files, each serving a specific purpose:

check_env_command.c: Handles the env command to display the current environment.
check_exit_command.c: Manages the exit command, allowing the user to exit the shell with an optional status code.
execute_command.c: Executes a given command using fork and execve.
get_user_input.c: Prompts the user for input and reads from standard input.
interactive_mode.c: Implements the interactive mode of the shell.
main.c: The main entry point of the GOAT Shell program, determining whether to run in interactive or non-interactive mode.
non_interactive_mode.c: Implements the non-interactive mode of the shell, executing commands from a script.
parse_input.c: Tokenizes user input into commands and arguments.
main.h: Header file containing function declarations and necessary includes.
Usage
To run the GOAT Shell interactively, simply execute the binary:

bash
./goat_shell
For non-interactive mode, provide a script file as an argument:
bash
./goat_shell script_file
Features
Interactive and Non-Interactive Modes: Choose between real-time interaction and script execution.
Command Execution: Execute commands using fork and execve.
Exit Command: Gracefully handle the exit command with optional status codes.
Environment Command: View the current environment using the env command.
Contributing
Feel free to contribute to the development of GOAT Shell. If you encounter issues or have suggestions, please open an issue on the GitHub repository.