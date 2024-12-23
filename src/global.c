#include "../include/global.h"

/*
    Call this function whenever you need to access
    global state of the shell.

    Using the Singleton design pattern 
 */
ShellState* get_shell_state(void) {
    static ShellState state;
    return &state;
}