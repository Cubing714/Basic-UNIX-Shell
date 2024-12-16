#ifndef COMMANDS_H
#define COMMANDS_H

#include "../include/utils.h"
#include "../include/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <bits/waitflags.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>
/*
List of built-in commands and their corresponding functions
*/

int lsh_num_builtins();
int lsh_cd(char** args);
int lsh_help(char** args);
int lsh_exit(char** args);
int lsh_pwd(char** args);
int lsh_echo(char** args);
int lsh_ls(char** args);
int lsh_mkf(char** args);
int lsh_find(char** args);
int lsh_mkdir(char** args);
int lsh_clear(char** args);
int lsh_rm(char** args);


extern char* builtin_str[];

// Array of pointers to builtin functions
extern int (*builtin_func[]) (char**);

#endif