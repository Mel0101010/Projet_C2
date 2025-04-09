#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"

int task_execve(char *cmd[]) {
    char *task = cmd[0];
    char *execve = cmd[1];
    char *UID = cmd[2];
    char *command = cmd[3];
    char *destination = cmd[4];
    FILE *fp;
    char buffer[1024];
    char command_output[8192] = "";
    char command_entier[1024];

    snprintf(command_entier, sizeof(command_entier), "%s %s", command, destination);

    fp = popen(command_entier, "r");
    if (fp == NULL) {
        printf("EXECVE,%s,ERROR\n", UID);
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(command_output, buffer);
    }
    pclose(fp);

    char *encoded_command = NULL;
    char *encoded_destination = NULL;
    char *encoded_output = NULL;
    
    encoder_base64(command, &encoded_command);
    encoder_base64(destination, &encoded_destination);
    encoder_base64(command_output, &encoded_output);
    
    char *newline;
    if ((newline = strchr(encoded_command, '\n'))) *newline = '\0';
    if ((newline = strchr(encoded_destination, '\n'))) *newline = '\0';
    if ((newline = strchr(encoded_output, '\n'))) *newline = '\0';

    printf("EXECVE,%s,%s,%s,%s\n", UID, encoded_command, encoded_destination, encoded_output);

    free(encoded_command);
    free(encoded_destination);
    free(encoded_output);

    return 0;
}

// Fonction pour diviser une chaîne en tokens selon un délimiteur
char** split_string(char* input, const char* delimiter, int* count) {
    *count = 0;
    char* copy = strdup(input);
    char* token = strtok(copy, delimiter);
    
    // Compter le nombre de tokens
    while (token) {
        (*count)++;
        token = strtok(NULL, delimiter);
    }
    
    // Allocation du tableau de résultats
    char** result = (char**)malloc((*count + 1) * sizeof(char*));
    
    // Refaire le tokenizing pour remplir le tableau
    strcpy(copy, input);
    token = strtok(copy, delimiter);
    int i = 0;
    
    while (token) {
        result[i] = strdup(token);
        i++;
        token = strtok(NULL, delimiter);
    }
    
    result[i] = NULL; // Terminer par NULL pour compatibilité avec exec
    free(copy);
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s \"EXECVE,UID,ENCODED_COMMAND,ENCODED_DESTINATION\"\n", argv[0]);
        return 1;
    }
    
    // Séparer l'entrée par les virgules
    int parts_count;
    char** parts = split_string(argv[1], ",", &parts_count);
    
    if (parts_count != 4 || strcmp(parts[0], "EXECVE") != 0) {
        printf("Format invalide. Attendu: EXECVE,UID,ENCODED_COMMAND,ENCODED_DESTINATION\n");
        for (int i = 0; i < parts_count; i++) {
            free(parts[i]);
        }
        free(parts);
        return 1;
    }
    
    // Récupérer les parties
    char* task_type = parts[0];       // "EXECVE"
    char* uid = parts[1];             // l'UID
    char* encoded_cmd = parts[2];     // commande encodée
    char* encoded_dest = parts[3];    // destination encodée
    
    // Décoder les parties encodées en base64
    char* command = NULL;
    char* destination = NULL;
    
    decoder_base64(encoded_cmd, &command);
    decoder_base64(encoded_dest, &destination);
    
    // Construire le tableau de commande pour task_execve
    char* cmd[6];
    cmd[0] = "task";
    cmd[1] = "execve";
    cmd[2] = uid;
    cmd[3] = command;
    cmd[4] = destination;
    cmd[5] = NULL;
    
    // Exécuter la tâche
    int result = task_execve(cmd);
    
    // Libérer la mémoire
    for (int i = 0; i < parts_count; i++) {
        free(parts[i]);
    }
    free(parts);
    free(command);
    free(destination);
    
    return result;
}