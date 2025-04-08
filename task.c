#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int task_sleep(char cmd[]) {
    srand(time(NULL));
    char* premier_mot = strtok(cmd, ",");
    char* deuxieme_mot = strtok(NULL, ",");
    if (!premier_mot || strcmp(premier_mot, "task") != 0 ||
        !deuxieme_mot || strcmp(deuxieme_mot, "sleep") != 0) {
        printf("Commande invalide.\n");
        return 1;
    }
    char* temps_texte = strtok(NULL, ",");
    if (!temps_texte) {
        printf("Temps manquant.\n");
        return 1;
    }

    int temps_base = strtol(temps_texte, NULL, 10);
    char* variation_texte = strtok(NULL, ",");
    int variation_max;
    if (variation_texte) {
        variation_max = strtol(variation_texte, NULL, 10);
    } else {
        variation_max = 0;
    }

    int valeur_variation = 0;
    if (variation_max > 0) {
        valeur_variation = (rand() % (2 * variation_max + 1)) - variation_max;
    }

    int temps_total = temps_base + valeur_variation;
    if (temps_total < 0) {
        temps_total = 0;
    }

           temps_total, temps_base, valeur_variation;

    return temps_total;
}

int main() {
    sleep(task_sleep("task,sleep,AZE23,12,2"));
}