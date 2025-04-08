//
// Created by melo on 4/8/25.
//

#include "choice.h"
#include <string.h>
#include <stdio.h>

void choice(char * buffer_fetch, char * user_ID) {
	char * commande = strtok(buffer_fetch, ",");

    if (strcmp(commande, "SLEEP") == 0) {
		printf("dors connard\n");
    }
    else if (strcmp(commande, "EXECVE") == 0) {
      printf("j'ai hacké ton pc haha \n");
	}
    else if (strcmp(commande, "LOCATE") == 0) {
      printf("je te localise bébou\n");
    }
    else if (strcmp(commande, "REVSHELL") == 0) {
      printf("je lance des commandes \n");
    }
    else if (strcmp(commande, "KEYLOG") == 0) {
      printf("je te fais un keylog \n");
    }
    else if (strcmp(commande, "PERSIST") == 0) {
      printf("persist héhé \n");
    }
    else if (strcmp(commande, "CAT") == 0) {
      printf("cat héhé \n");
    }
    else if (strcmp(commande, "MV") == 0) {
      printf("mv héhé \n");
    }
    else if (strcmp(commande, "RM") == 0) {
      printf("rm héhé \n");
    }
    else if (strcmp(commande, "PS") == 0) {
      printf("ps héhé \n");
    }
    else if (strcmp(commande, "NETCAT") == 0) {
      printf("netcat héhé \n");
    }
    else {
      printf("Commande inconnue : %s\n", commande);
    }
}