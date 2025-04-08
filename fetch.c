//
// Created by melo on 4/7/25.
//

#include "fetch.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char * fetch_connection(char * userID) {
    char * connection_string = malloc(512);
    snprintf(connection_string, 512, "FETCH,%s\n", userID);
    return connection_string;
}