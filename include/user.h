#ifndef USER_H
#define USER_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_PASS_LEN 50
typedef struct User {
    char name[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    const char* home_dir;
    int privilege;
} User;

User* create_user(const char* name, const char* password, const char* home_dir, int privilege);
void save_user_data();
void load_user_data(const char* filename, User*** users, int* num_users);


#endif



