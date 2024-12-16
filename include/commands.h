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

#define PATH_MAX 4096

// Struct that hold command data
typedef struct Command {
    char* name;
    int privilege; // 0 is privilege mode, 1 is user mode
    int (*func)(char**); // Function pointer to each command
    // Maybe also add a string for help command that describe what each command does
} Command;

// For reference around shell
extern Command builtin_commands[];

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


#endif