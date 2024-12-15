#include "../include/shell.h"

void lsh_loop(void) {
    char* line;
    char ** args;
    int status;

    do {
        printf("\n%s: > ", get_cwd());
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);
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

int lsh_launch(char** args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        // args[0] is the program name
        // args is the rest of the arguments
        // execvp expects a program name and a array/string of arguments
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("lsh");
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        // WIFEXITED checks if the process was terminated normally
        // WIFSIGNALED checks if the process was terminated by signal
    }
    
    return 1;
}

int lsh_execute(char** args) {
    if (args[0] == NULL) {
        // An empty command was entered
        return 1;
    }

    for (int i = 0; i < lsh_num_builtins(); ++i) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
}

#define GET_CWD_BUFSIZE 1024
char* get_cwd(void) {
    static int bufsize = GET_CWD_BUFSIZE;
    char* buffer = malloc(bufsize * sizeof(char));
    if (getcwd(buffer, bufsize) == NULL) {
        perror("lsh");
        free(buffer);
        exit(EXIT_FAILURE);
    }
    return buffer;
}