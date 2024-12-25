#include "../include/global.h"

/*
    Call this function whenever you need to access
    global state of the shell.

    Using the Singleton design pattern 
 */
ShellState* get_shell_state(void) {
    static ShellState* state;
    
    // Initialize the state if it hasn't been initialized
    if (state == NULL) {
        state = malloc(sizeof(ShellState));
        if (state == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        state->project_dir = malloc(PATH_MAX * sizeof(char));
        state->users = NULL;
        state->current_user = NULL;
        state->num_users = 0;
    }
    
    return state;
}