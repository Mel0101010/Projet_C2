//
// Created by melo on 4/7/25.
//
#include "declare.h"
#include "fetch.h"
#include "connexion.h"
#include "choice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_ADDR "127.0.0.1"


int main() {
    char buffer[1024] = {0};
    char declare[1024] = {0};
    char user_ID[256] = {0};
    int sock;
    int request_count = 50;

    // Partie 1 : Déclaration de la connexion
	while (1) {
        sleep(5);
        if (request_count >= 50) {
          	sock = sock_init();
			if (sock == -1) {
		    	return EXIT_FAILURE;
			}
			strcpy(declare,declare_connection());
			send_message(sock, declare, buffer);
			strtok(buffer, ",");
			strcpy(user_ID,strtok(NULL, ","));
			close(sock);
            request_count = 0;
        }


    // Partie 2 : Récupération de l'ordre

    	int sock_fetch = sock_init();
    	if (sock_fetch == -1) {
    	    return EXIT_FAILURE;
    	}
    	char * commande_fetch = fetch_connection(user_ID);
    	printf("Commande FETCH : %s\n", commande_fetch);
    	char * buffer_fetch = malloc(1024);
		send_message(sock_fetch, commande_fetch, buffer_fetch);
    	printf("Message du serveur : %s\n", buffer_fetch);
    	close(sock_fetch);

		choice(buffer_fetch, user_ID);


        free(buffer_fetch);
    }
    return EXIT_SUCCESS;
}