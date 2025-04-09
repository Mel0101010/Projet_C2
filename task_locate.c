#include <stdio.h>
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
        return 1; // Retourne une erreur si l'ouverture échoue
    }
    // Lire la réponse de curl
    if (fgets(latitude_longitude, sizeof(latitude_longitude), fp) != NULL) {
        //fgets et sizeof pour éviter l'overflow
        // Enlever le saut de ligne à la fin de la chaîne
        //je met au propre le résultat de latitude et longitude
        // Afficher la sortie au format attendu
        //ca renvoie que une valeur
    }
    return 0;
}
}
