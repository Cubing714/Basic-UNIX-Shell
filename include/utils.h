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

void rm_file(const char* filename); // Remove a file
void rm_directory(const char* dir_name); // Remove an empty dir
void rm_recursive(const char* path); // Remove a dir and its contents
char* set_project_dir(void);
char* strip_home_suffix(char* project_dir);

#endif