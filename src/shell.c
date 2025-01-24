#include "../include/shell.h"
#include "../include/global.h"

#define MAX_PATH_SIZE 1024
void lsh_loop(void) {
    char* line;
    char** args;
    int status;

    ShellState* g_state = get_shell_state();

    init_shell();

    do {
        if (g_state->current_user != NULL) {
            printf("\nSimpleShell:%s:%s > ", g_state->current_user->name, get_cwd_display(g_state->project_dir));
        } 


        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);

    // Free memory allocated for users
    // Will also free current user
    for (int i = 0; i < g_state->num_users; ++i) {
        free(g_state->users[i]);
    }

    free(g_state->users);
}

#define LSH_RL_BUFSIZE 1024
char* lsh_read_line(void) {         
    int bufsize = LSH_RL_BUFSIZE;     
    int position = 0;
    char* buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read character 
        c = getchar();
        
        // If we hit EOF, replace with a null character and return
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate
        if (position >= bufsize) {
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char** lsh_split_line(char* line) {
    int bufsize = LSH_TOK_BUFSIZE;
    char** tokens = malloc(sizeof(char*) * bufsize);
    char* token;
    int position = 0;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int lsh_execute(char** args) {
    if (args[0] == NULL) {
        // An empty command was entered
        return 1;
    }

    for (int i = 0; i < lsh_num_builtins(); ++i) {
        if (strcmp(args[0], builtin_commands[i].name) == 0) {
            return (*(builtin_commands[i].func))(args);
        }
    }

    printf("Command: \"%s\" not recognized\n", args[0]);
    return 1;
}

void init_shell(void) {

    ShellState* g_state = get_shell_state();

    // Create user data file if it doesn't exists
    if (!file_exists(USER_DATA_FILE)) {
        int fd = open(USER_DATA_FILE, O_CREAT, 0644);
        if (fd == -1) {
            perror("lsh: error creating file");
        }
        close(fd);
    } 

    if (!is_file_empty(USER_DATA_FILE)) {
        load_user_data(USER_DATA_FILE, &g_state->users, &g_state->num_users);
        create_guest_user(&g_state->users, &g_state->num_users);
    } else {
        char* name;
        char* pass;

        int capacity = 10; // Arbitrary number for space of users
        g_state->num_users = 0;
        g_state->users = malloc(capacity * sizeof(User));
        if (!g_state->users) {
            perror("lsh: allocation error\n");
            exit(EXIT_FAILURE);
        }

        // Create the first root user
        printf("No users detected please create root user.\n");
        printf("Username: ");
        name = lsh_read_line();

        disable_echo();
        printf("Password: ");
        pass = lsh_read_line();
        enable_echo();
        
        User* root_user = create_user(name, pass, 1);
        
        g_state->current_user = root_user;
        g_state->users[0] = root_user;
        g_state->num_users++;
    }

    if (getcwd(g_state->project_dir, PATH_MAX) == NULL) {
        perror("lsh: error getting project directory");
        exit(EXIT_FAILURE);
    }

    // Set cwd to users home directory
    if (g_state->current_user != NULL) {
        if (chdir(g_state->current_user->home_dir) != 0) {
            perror("lsh: error changing to user home directory");
            exit(EXIT_FAILURE);
        }
    }
}

#define GET_CWD_BUFSIZE 1024
char* get_cwd_display(const char* project_dir) {
    char* buffer = malloc(GET_CWD_BUFSIZE * sizeof(char));
    if (buffer == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    if (getcwd(buffer, GET_CWD_BUFSIZE) == NULL) {
        perror("getcwd failed");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    static char relative_path[GET_CWD_BUFSIZE] = "";

    // Check if the current directory starts with the project directory path
    if (strncmp(buffer, project_dir, strlen(project_dir)) == 0) {
        // Calculate the relative path from the project directory
        snprintf(relative_path, GET_CWD_BUFSIZE, "%s", buffer + strlen(project_dir));
        if (relative_path[0] == '\0') {
            // If in the project directory itself, use "/"
            snprintf(relative_path, GET_CWD_BUFSIZE, "/");
        }
    } else {
        // If not inside the project directory, return the absolute path
        snprintf(relative_path, GET_CWD_BUFSIZE, "%s", buffer);
    }

    free(buffer);
    return relative_path;
}