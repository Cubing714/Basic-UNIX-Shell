#include "../include/user.h"
#include "../include/shell.h"

User* create_user(const char* name, const char* password, const char* home_dir, int privilege) {
    User* user = malloc(sizeof(User));
    if (user == NULL) {
        perror("lsh: allocation failed");
        exit(EXIT_FAILURE);
    }

    strncpy(user->name, name, MAX_NAME_LEN - 1);
    user->name[MAX_NAME_LEN - 1] = '\0';

    strncpy(user->password, password, MAX_PASS_LEN - 1);
    user->password[MAX_PASS_LEN - 1] = '\0';


    user->home_dir = home_dir; 
    user->privilege = privilege;
    return user;
}

void save_user_data() {

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

    while (fscanf(file, "%49s %49s %d", name, password, &privilege) == 3) {
        User* new_user = create_user(name, password, "./home", privilege);
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

        //free(new_user);

    }

    fclose(file);
}