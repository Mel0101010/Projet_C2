#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int task_locate(char *cmd[]) {
    char buffer[1024]; //nb d'octes MAX
    FILE *fp; //FILE est utilise pour ouvrir un flux vers un lien externe
    char latitude_longitude[100];
    char *task = cmd[0];
    char *locate = cmd[1];
    char *UID = cmd[2];
    // j'ai décomper les commandes dans cmd[]
    // char *cmd[] = { "task", "locate", "cd2e75849f", NULL };
    // le pointer FILE fp* j'ai pris les fonctions de FILE

    // Exécuter la requête curl pour obtenir la géolocalisation
    fp = popen("curl -s http://ipinfo.io/loc", "r");
    //popen() est une fonction qui permet exécuter directement la commande dans le terminal et dedans doit être en chaine de caractere
    //c'est comme si tu était dans un terminal
    if (fp == NULL) { // Vérification si le flux a bien été ouvert
        fprintf(stderr, "Erreur lors de l'exécution de la commande.\n");
        return 1; // Retourne une erreur si l'ouverture échoue
    }
    // Lire la réponse de curl
    if (fgets(latitude_longitude, sizeof(latitude_longitude), fp) != NULL) {
        //fgets et sizeof pour éviter l'overflow
        // Enlever le saut de ligne à la fin de la chaîne
        //je met au propre le résultat de latitude et longitude
        // Afficher la sortie au format attendu
        //ca renvoie que une valeur
        latitude_longitude[strcspn(latitude_longitude, "\n")] = 0;
        printf("LOCATE_RESULT,%s,%s\n", UID, latitude_longitude);
    } else {
        printf("LOCATE_ERROR,%s,Impossible d'obtenir la localisation\n", UID);
    }
    pclose(fp);
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
    // Mode par défaut (pour test)
    if (argc == 1) {
        char *cmd[] = { "task", "locate", "cd2e75849f", NULL };
        return task_locate(cmd);
    }
    
    // Mode avec argument d'entrée "LOCATE,UID"
    if (argc == 2) {
        int parts_count;
        char** parts = split_string(argv[1], ",", &parts_count);
        
        if (parts_count != 2 || strcmp(parts[0], "LOCATE") != 0) {
            printf("Format invalide. Attendu: LOCATE,UID\n");
            for (int i = 0; i < parts_count; i++) {
                free(parts[i]);
            }
            free(parts);
            return 1;
        }
        
        // Récupérer l'UID
        char* uid = parts[1];
        
        // Construire le tableau de commande pour task_locate
        char* cmd[4];
        cmd[0] = "task";
        cmd[1] = "locate";
        cmd[2] = uid;
        cmd[3] = NULL;
        
        // Exécuter la tâche
        int result = task_locate(cmd);
        
        // Libérer la mémoire
        for (int i = 0; i < parts_count; i++) {
            free(parts[i]);
        }
        free(parts);
        
        return result;
    }
    
    // Mauvais nombre d'arguments
    printf("Usage: %s [LOCATE,UID]\n", argv[0]);
    return 1;
}
