//
// Created by melo on 4/7/25.
//

#include "declare.h"


#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <pwd.h>

char * declare_connection() {
    // Initialize the connection
    char username[256] = {0};
    char hostname[256] = {0};
    char os[256] = {0};

    // Get username
    struct passwd *pwd = getpwuid(getuid());
    if (pwd != NULL) {
        strncpy(username, pwd->pw_name, sizeof(username) - 1);
    }
    else {
        strcpy(username, "unknown");
    }

    // Get hostname
    if (gethostname(hostname, sizeof(hostname) - 1) != 0) {
        strcpy(hostname, "localhost");
    }

    // Get OS information
    struct utsname system_info;
    if (uname(&system_info) == 0) {
        snprintf(os, sizeof(os) - 1, "%s %s", system_info.sysname, system_info.release);
    }
    else {
        strcpy(os, "linux");
    }
    printf("Username: %s\n", username);
    printf("Hostname: %s\n", hostname);
    printf("OS Info: %s\n", os);

    char *connection_string = malloc(512);
    if (connection_string == NULL) {
        perror("Failed to allocate memory for connection string");
        return NULL;
    }

    snprintf(connection_string, 512, "DECLARE,%s,%s,%s\n", username, hostname, os);
    return connection_string;
}