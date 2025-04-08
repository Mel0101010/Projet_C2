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

char * sleep_task(char * buffer_fetch) {
	/*
	Tache qui coupe le programme pendant X secondes sur demande du C2
	format de l'entrée : SLEEP,e2f6421cfa,MQ==,MA==
	format de la sortie : e2f6421cfa : ID de la tache
	*/
	char buffer[1024];
	strcpy(buffer, buffer_fetch); // Create a copy to avoid modifying original string

	char * commande = strtok(buffer, ",");
	if (strcmp(commande, "SLEEP") != 0) {
		printf("Invalid command\n");
		return NULL;
	}

	char * user_ID = strtok(NULL, ",");
	char * user_ID_copy = strdup(user_ID); // Create a persistent copy of user_ID

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

	// Generate a random number between timer-random and timer+random
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
	return user_ID_copy;
}