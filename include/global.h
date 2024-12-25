#ifndef GLOBAL_H
#define GLOBAL_H

#include "./user.h"

typedef struct ShellState {
    char* project_dir;
    User** users; 
    User* current_user;
    int num_users;
} ShellState;

ShellState* get_shell_state(void);

#endif
