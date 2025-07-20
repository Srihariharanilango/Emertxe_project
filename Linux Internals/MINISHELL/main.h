#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3


#endif
extern int status;


void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);

