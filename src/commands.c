#include "../include/commands.h"

char* builtin_str[] = {
    "cd",
    "help",
    "exit",
    "pwd",
    "echo",
    "ls",
    "mkf",
    "find",
    "mkdir",
    "clear",
    "rm"
};

// Array of pointers to builtin functions
int (*builtin_func[]) (char**) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit,
    &lsh_pwd,
    &lsh_echo,
    &lsh_ls,
    &lsh_mkf,
    &lsh_find,
    &lsh_mkdir,
    &lsh_clear,
    &lsh_rm
};

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char*);
}

int lsh_cd(char** args) {
    if(args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to\"cd\"\n" );
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char** args) {
    printf("Eric's Linux Shell\n");
    printf("Type program names and arguments, and hit enter\n");
    printf("The following are built in\n");

    for (int i = 0; i < lsh_num_builtins(); ++i) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int lsh_exit(char** args) {
    return 0;
}

#define LSH_PWD_BUFSIZE 1024
int lsh_pwd(char** args) {
    int bufsize = LSH_PWD_BUFSIZE;
    char* buffer = malloc(bufsize * sizeof(char));
    if (getcwd(buffer, bufsize) == NULL) {
        perror("lsh");
        free(buffer);
        return 1;
    }
    printf("%s", buffer);
    free(buffer);
    return 1;
}

#define LSH_ECHO_BUFSIZE 1024
int lsh_echo(char** args) {
    int bufsize = LSH_ECHO_BUFSIZE;
    char* buffer = malloc(bufsize * sizeof(char));

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Defined before in case we add flags like -n for no newline
    int i = 1; // Start at index to skip echo command
    
    for(; args[i] != NULL; ++i) {
        printf("%s", args[i]);
        if (args[i + 1] != NULL) {
            printf(" ");
        }
    }

    free(buffer);
    return 1;
}

int lsh_ls(char** args) {
    DIR* dir;
    struct dirent* dr;

    if (args[1] == NULL) {
        dir = opendir(".");
        // Print out current dir 
        printf("[ ");
        lsh_pwd(args);
        printf(" ]\n");

    } else {
        dir = opendir(args[1]);
        printf("[ ");
        printf("%s", args[1]);
        printf(" ]\n");
    }


    if (!dir) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }


    while ((dr = readdir(dir))) {
        // Skip the current (.) and parent (..) directory entries
        if (strcmp(dr->d_name, ".") != 0 && strcmp(dr->d_name, "..") != 0){
            printf("\t%s\n", dr->d_name);
        }
    }

    closedir(dir);
    return 1;
}

int lsh_mkf(char** args) {
    char* filename = args[1];
    struct stat buffer;

    if (filename == NULL) {
        fprintf(stderr, "lsh: expected argument to\"mf\"\n" );
    } else {
        if (stat(filename, &buffer) == 0) {
            printf("File already exists: %s", filename);
            return 1;
        }
        
        int fd = open(filename, O_CREAT | O_WRONLY, 0644);
        if (fd == -1) {
            perror("lsh: error creating file");
            return 0;
        }

        printf("File created: %s", filename);
    }

    return 1;
}

int lsh_find(char** args) {
    DIR* dir;
    struct dirent* dr;

    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected arguments");
        return 1;
    }

    dir = opendir(args[1]);
    if (!dir) {
        perror("opendir");
        return 1;
    }
    

    for (int i = 2; args[i] != NULL; ++i) {
        int found = 0;

        // Loop over all entries in the directory
        while ((dr = readdir(dir))) {
            // If a match is found, set found to 1 and break the loop
            if (strcmp(dr->d_name, args[i]) == 0) {
                found = 1;
                break;
            }
        }

        // Print whether the file was found or not
        if (found) {
            printf("Found: %s\n", args[i]);
        } else {
            printf("Not found: %s\n", args[i]);
        }

        // Reset directory pointer to the start of the directory for the next file name
        rewinddir(dir);
    }
    // Close dir always
    closedir(dir);

    return 1;
}

int lsh_mkdir(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: mkdir: missing operand\n");
        return 1;
    }

    if (mkdir(args[1], 0777) == -1) {
        perror("lsh");
        return 1;
    }

    printf("Directory '%s' created successfully.\n", args[1]);
    return 1;
}

int lsh_clear(char** args) {
    system("clear");
    return 1;
}

int lsh_rm(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"rm\"\n");
        return 1;
    }

    if (strcmp(args[1], "-r") == 0 && args[2] != NULL) {
        // Remove directory recursively
        rm_recursive(args[2]);
    } else {
        rm_file(args[1]);
    }
    return 1;
}