#include "../include/utils.h"

void rm_file(const char* filename) {
    if (remove(filename) == 0) {
        printf("Removed file: %s\n", filename);
    } else {
        perror("lsh: error removing file");
    }
}

void rm_directory(const char* dir_name) {
    if (rmdir(dir_name) == 0) {
        printf("Removed directory: %s\n", dir_name);
    } else {
        perror("lsh");
    }
}

#define PATH_BUF_SIZE 1024
void rm_recursive(const char* path) {
    DIR* dir = opendir(path);
    struct dirent* dir_ent;

    if (dir == NULL) {
        perror("lsh: unable to open directory");
        return;
    }

    // Traverse directory and remove files/subdirectories   
    while ((dir_ent = readdir(dir)) != 0) {
        if (strcmp(dir_ent->d_name, ".") == 0 || strcmp(dir_ent->d_name, "..") == 0) {
            continue;
        }

        // Make full path to the entry
        char full_path[PATH_BUF_SIZE];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, dir_ent->d_name);

        struct stat statbuf;
        if (stat(full_path, &statbuf) == 0) {
            // Check if its a directory
            if(S_ISDIR(statbuf.st_mode)) {
                rm_recursive(full_path);
            } else {
                rm_file(full_path);
            }
        }

    
    }

    closedir(dir);

    // Now that directory is empty and closed it removes itself
    rm_directory(path);
} 

char* set_project_dir(void) {
    // Get the absolute path of the project directory 
    char* project_dir = realpath("./", NULL);
    if (project_dir == NULL) {
        perror("Failed to get absolute path of the project directory");
        exit(EXIT_FAILURE);
    }
    
    return project_dir;
}

char* strip_home_suffix(char* project_dir) {
    const char* home_suffix = "/home";
    size_t project_len = strlen(project_dir);
    size_t suffix_len = strlen(home_suffix);

    // Check if `project_dir` ends with `/home`
    if (project_len >= suffix_len && strcmp(project_dir + project_len - suffix_len, home_suffix) == 0) {
        // Null-terminate the string to strip `/home`
        project_dir[project_len - suffix_len] = '\0';
    }

    return project_dir;
}

int compare_strings(const void* a, const void* b) {
    // Cast the void pointers to string pointers
    const char *str_a = *(const char**)a;
    const char *str_b = *(const char**)b;

    return strcmp(str_a, str_b);
}

int file_exists(const char* filename) {
    return access(filename, F_OK) == 0;
}

int is_file_empty(const char* filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        perror("Error getting file stats");
        return -1;
    }
    return (st.st_size == 0);
}

int dir_exists(const char* dir_name) {
    struct stat st;
    return stat(dir_name, &st) == 0 && S_ISDIR(st.st_mode);
}