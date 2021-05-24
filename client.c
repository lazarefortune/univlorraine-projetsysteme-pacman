#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#define PORT 6000
#define MAX_BUFFER 4096

const char *EXIT = "f";

char tampon[MAX_BUFFER];

void playerChoice(char tampon[]) {
    printf("z : haut | q : gauche | d : droite | s : bas | f : Fin du jeu\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[]) {
    return strcmp(tampon, EXIT) == 0;
}

//Création de la socket
int initSocket(){
    int fdSocket = socket(PF_INET, SOCK_STREAM, 0);
    if(fdSocket<0){
        printf("Erreur lors de la création de socket\n");
        return EXIT_FAILURE;
    }
    return fdSocket;
}

//Création de la structure d'adresse du serveur
struct sockaddr_in initAddr(){
    struct sockaddr_in coord;
    int longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coord, 0x00, longueurAdresse);
    coord.sin_family = PF_INET;
    inet_aton("127.0.0.1", &coord.sin_addr);
    coord.sin_port = htons(PORT);
    return coord;
}

//Connexion au serveur
void serverConnect(int fdSocket){
    struct sockaddr_in serverCoord = initAddr();
    if(connect(fdSocket, (struct sockaddr *) &serverCoord, sizeof(serverCoord)) == -1) {
        printf("Connexion impossible\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Connexion au serveur de jeu réussie !\n");
    }
}


bool playerIsAlive(){
     for (int i = 0; i < strlen(tampon); i++)
     {
         if (tampon[i] == 'X')
         {
             return false;
         }
     }
    return true;
     
}

bool existFruit(){
     for (int i = 0; i < strlen(tampon); i++)
     {
         if (tampon[i] == '*')
         {
             return true;
         }
     }
    return false;
     
}

int main(int argc , char const *argv[]) {
    int fdSocket;
    struct sockaddr_in coordonneesServeur;

    int nbRecu;
    int longueurAdresse;
    

    fdSocket = initSocket();
    
    if (fdSocket < 0) {
        printf("socket incorrecte\n");
        exit(EXIT_FAILURE);
    }

    serverConnect(fdSocket);
    
    

    while (1) {
        nbRecu = recv(fdSocket, tampon, MAX_BUFFER, 0);
        if (nbRecu > 0) {
            tampon[nbRecu] = 0;
            printf("%s\n", tampon);
        }
        // on verifie s'il est vivant
        if (!playerIsAlive())
        {
            printf("Vous avez perdu \n");
            break;
        }

        if (!existFruit())
        {
            printf("Felicitations !!!  Vous avez mangé tous les fruits\n");
            break;
        }

        playerChoice(tampon);

        // on envoie le message au serveur
        send(fdSocket, tampon, strlen(tampon), 0);
        
        if (testQuitter(tampon)) {
            printf("Vous avez mis fin à la partie \n");
            break;
        }

        
        
    }

    close(fdSocket);

    return EXIT_SUCCESS;
}
