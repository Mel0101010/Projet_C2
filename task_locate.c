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
        fprintf(fp, "Erreur lors de l'exécution de la commande.\n");
        return 1; // Retourne une erreur si l'ouverture échoue
    }
    // Lire la réponse de curl
    if (fgets(latitude_longitude, sizeof(latitude_longitude), fp) != NULL) {
        //fgets et sizeof pour éviter l'overflow
        // Enlever le saut de ligne à la fin de la chaîne
        //je met au propre le résultat de latitude et longitude
        // Afficher la sortie au format attendu
        //ca renvoie que une valeur
        printf("LOCATE_RESULT,%s,%s\n", UID ,latitude_longitude); //tu peux retirer
    }
    return 0;
}
int main() {
    char *cmd[] = { "task", "locate", "cd2e75849f", NULL };// le NULL je ne veux pas plus de 3 éléments
    task_locate(cmd); // Appel de la fonction task_locate pour afficher la localisation
    return 0;
}
