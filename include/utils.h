// Utility functions
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>

void rm_file(const char* filename); // Remove a file
void rm_directory(const char* dir_name); // Remove an empty dir
void rm_recursive(const char* path); // Remove a dir and its contents
char* set_project_dir(void);
char* strip_home_suffix(char* project_dir);
int compare_strings(const void* a, const void* b);
int file_exists(const char* filename);
int is_file_empty(const char* filename);
int dir_exists(const char* dir_name);
void disable_echo(void);
void enable_echo(void);


#endif