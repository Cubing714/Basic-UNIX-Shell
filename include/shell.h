#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "./commands.h"
#include "./user.h"

#define MAX_NUM_DIRECTORIES 100
#define MAX_FILENAME_CHAR_LEN 255

// Global Variables [ Please dont be mad at me ]
extern const char* g_project_dir;
extern User** g_users; 
extern User* g_current_user;
extern int g_num_users;


void lsh_loop(void);
char* lsh_read_line(void);
char** lsh_split_line(char* line);
int lsh_launch(char** args);
int lsh_execute(char** args);
char* init_shell_directory(void);
char* get_cwd_display(const char* project_dir);

#endif