#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "commands.h"

void lsh_loop(void);
char* lsh_read_line(void);
char** lsh_split_line(char* line);
int lsh_launch(char** args);
int lsh_execute(char** args);
char* get_cwd(void);

#endif