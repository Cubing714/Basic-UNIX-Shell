#ifndef USER_H
#define USER_H

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./utils.h"

#define USER_DATA_FILE "./config/user.dat"
#define MAX_NAME_LEN 50
#define MAX_PASS_LEN 50

typedef struct User {
    char name[MAX_NAME_LEN];
    char password[MAX_PASS_LEN];
    char* home_dir;
    int privilege;
} User;

User* create_user(const char* name, const char* password, int privilege);
void save_user_data();
void load_user_data(const char* filename, User*** users, int* num_users);
void create_guest_user(User*** users, int* num_users);


#endif



