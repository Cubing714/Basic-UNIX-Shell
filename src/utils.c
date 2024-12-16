#include "../include/utils.h"

void rm_file(const char* filename) {
    if (remove(filename) == 0) {
        printf("Removed file: %s", filename);
    } else {
        perror("lsh: error removing file");
    }
}

void rm_directory(const char* dir_name) {
    if (rmdir(dir_name) == 0) {
        printf("Removed directory: %s", dir_name);
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
    printf("Removed directory: %s\n", path);
} 