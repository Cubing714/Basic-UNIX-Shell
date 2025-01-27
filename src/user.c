#include "../include/user.h"
#include "../include/shell.h"
#include "../include/global.h"

User* create_user(const char* name, const char* password, int privilege) {
    User* user = malloc(sizeof(User));
    if (user == NULL) {
        perror("lsh: allocation failed");
        exit(EXIT_FAILURE);
    }

    strncpy(user->name, name, MAX_NAME_LEN - 1);
    user->name[MAX_NAME_LEN - 1] = '\0';

    strncpy(user->password, password, MAX_PASS_LEN - 1);
    user->password[MAX_PASS_LEN - 1] = '\0';

    user->privilege = privilege;

    // Allocate memory for "./home/" + user_name + null terminator
    size_t len = strlen("./home/") + strlen(user->name) + 1;
    user->home_dir = malloc(len); 
    if (user->home_dir == NULL) {
        perror("lsh: allocation error");
        exit(EXIT_FAILURE);
    }

    strcpy(user->home_dir, "./home/");
    strcat(user->home_dir, user->name);
    
    if (!dir_exists(user->home_dir)) {
        if (mkdir(user->home_dir, 0700) == -1) {
            perror("lsh: failed to create home directory");
            exit(EXIT_FAILURE);
        } 
    }

    return user;
}

void save_user_data(void) {
    ShellState* g_state = get_shell_state();
    
    // get absolute path then check if it exists
    // Still trying to fix
    
    char user_dat_abs_path[PATH_MAX];
    strcpy(user_dat_abs_path, g_state->project_dir);
    strcat(user_dat_abs_path, "/config/user.dat");

    // Create user data file if it doesn't exists
    if (!file_exists(user_dat_abs_path)) {
        int fd = open(user_dat_abs_path, O_CREAT, 0644);
        if (fd == -1) {
            perror("lsh: error creating file");
        }
        close(fd);
    } 

    FILE* file = fopen(user_dat_abs_path, "w"); 
    if (file == NULL) {
        perror("lsh: failed to open file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < g_state->num_users; ++i) {
        User* user = g_state->users[i];
        if (!user->guest) {
            fprintf(file, "%s %s %d\n", user->name, user->password, user->privilege);
        }
    }

    fclose(file);
}

void load_user_data(const char* filename, User*** users, int* num_users) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("lsh: failed to open file\n");
        exit(EXIT_FAILURE);
    }

    int capacity = 10; // Arbitrary number for space of users
    *num_users = 0;
    *users = malloc(capacity * sizeof(User));
    if (!*users) {
        perror("lsh: allocation error\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char name[MAX_NAME_LEN]; 
    char password[MAX_PASS_LEN];
    int privilege;

    while (fscanf(file, "%s %s %d", name, password, &privilege) == 3) {
        User* new_user = create_user(name, password, privilege);
        if (new_user == NULL) {
            fprintf(stderr, "Failed to create new user\n");
            continue;
        }

         // Resize the array if necessary
        if (*num_users >= capacity) {
            capacity *= 2;  // Double the capacity

            User** temp = realloc(*users, capacity * sizeof(User)); // Temporary pointer for realloc
            if (temp == NULL) {
                perror("lsh: allocation error during resize");
                free(*users); // Free the old allocation
                exit(EXIT_FAILURE);
            }
            *users = temp;  // Update *users with the resized memory
        }

        (*users)[*num_users] = new_user;
        (*num_users)++; // Increment number of users
    }

    fclose(file);
}

void create_guest_user(User*** users, int* num_users) {
    User* guest = create_user("guest", "guest", 0);
    guest->guest = 1; // Set it to true
    guest->home_dir = "./home/guest";
    if (!dir_exists(guest->home_dir)) {
        if (mkdir(guest->home_dir, 0700) == -1) {
            perror("lsh: failed to create home directory");
            exit(EXIT_FAILURE);
        }
    }
    
    (*users)[*num_users] = guest;
    (*num_users)++;

    ShellState* g_state = get_shell_state();

    g_state->current_user = guest;
}