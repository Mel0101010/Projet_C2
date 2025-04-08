#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int task_sleep() {
    char cmd[] = "task,sleep,fcc3f516b7,5,2";
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
    sleep(temps_total);
    return 0;
}

int main() {
    int task_sleep();
    printf("\n",task_sleep());
}