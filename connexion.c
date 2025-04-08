//
// Created by melo on 4/8/25.
//

#include "connexion.h"

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