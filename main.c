//
// Created by melo on 4/7/25.
//
#include "declare.h"
#include "fetch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_ADDR "127.0.0.1"


int sock_init() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Créer un socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Erreur de création de socket \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr) <= 0) {
        printf("\nAdresse invalide / Adresse non supportée \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Erreur de connexion \n");
        return -1;
    }

    return sock;
}

char * send_message(int sock, char *message, char * buffer) {
    send(sock, message, strlen(message), 0);
    read(sock, buffer, 1024);
    return buffer;
}





int main() {
    char buffer[1024] = {0};

    // Partie 1 : Déclaration de la connexion

    int sock = sock_init();
    if (sock == -1) {
        return EXIT_FAILURE;
    }
    char *declare = declare_connection();
    send_message(sock, declare, buffer);
    strtok(buffer, ",");
    char * user_ID = strtok(NULL, ",");

    close(sock);

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
    return 0;
}