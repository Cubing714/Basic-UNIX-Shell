#include "../include/commands.h"
#include "../include/shell.h"

Command builtin_commands[] = {
    {"cd", 1, &lsh_cd},
    {"help", 1, &lsh_help},
    {"exit", 1, &lsh_exit},
    {"pwd", 1,  &lsh_pwd},
    {"echo", 1, &lsh_echo},
    {"ls", 1, &lsh_ls},
    {"mkf", 1, &lsh_mkf},
    {"find", 1, &lsh_find},
    {"mkdir", 1, &lsh_mkdir},
    {"clear", 1, &lsh_clear},
    {"rm", 1, &lsh_rm},
};


int lsh_num_builtins() {
    return sizeof(builtin_commands) / sizeof(Command);
}

int lsh_cd(char** args) {
    if(args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to\"cd\"\n" );
    } else {
        // Get the absolute path of the argument
        char abs_path[PATH_MAX];
        if (realpath(args[1], abs_path) == NULL)  {
            perror("lsh");
            return 1;
        }

        // Check if the target is inside the project directory
        if (strncmp(abs_path, project_dir, strlen(project_dir)) != 0) {
            fprintf(stderr, "lsh: permission denied - cannot leave shell directory\n");
            return 1;
        }

        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char** args) {
    printf("Eric's UNIX Shell\n");
    printf("Type program names and arguments, and hit enter\n");
    printf("The following are built in\n");

    for (int i = 0; i < lsh_num_builtins(); ++i) {
        printf("  %s\n", builtin_commands[i].name);
    }
    // Will uncomment when implemented
    // printf("Use the man command for information on other programs.\n");
    return 1;
}

int lsh_exit(char** args) {
    return 0;
}

#define LSH_PWD_BUFSIZE 1024
int lsh_pwd(char** args) {
    printf("%s", get_cwd_display(project_dir));
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

    char** d_name_arr = malloc(MAX_NUM_DIRECTORIES * sizeof(char*));
    int d_count = 0;

    char** f_name_arr = malloc(MAX_NUM_DIRECTORIES * sizeof(char*));
    int f_count = 0;

    while ((dr = readdir(dir))) {
        // Skip the current (.) and parent (..) directory entries
        if (strcmp(dr->d_name, ".") != 0 && strcmp(dr->d_name, "..") != 0){
            if (dr->d_type == DT_DIR) {
                d_name_arr[d_count] = malloc(MAX_FILENAME_CHAR_LEN * sizeof(char));
                strncpy(d_name_arr[d_count], dr->d_name, MAX_FILENAME_CHAR_LEN);
                d_count++;
            } else {
                f_name_arr[f_count] = malloc(MAX_FILENAME_CHAR_LEN * sizeof(char));
                strncpy(f_name_arr[f_count], dr->d_name, MAX_FILENAME_CHAR_LEN);
                f_count++;
            }
        }
    }

    qsort(d_name_arr, d_count, sizeof(char*), compare_strings);
    qsort(f_name_arr, f_count, sizeof(char*), compare_strings);

    printf("dir:\n");
    for (int i = 0; i < d_count; ++i) {
        printf(" \t%s\n", d_name_arr[i]);
    }

    printf("file:\n");
    for (int i = 0; i < f_count; ++i) {
        printf(" \t%s\n", f_name_arr[i]);
    }

    free(d_name_arr);
    free(f_name_arr);
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