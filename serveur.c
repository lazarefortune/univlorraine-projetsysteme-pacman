#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// #define HEIGHT 6
// #define WIDTH 10

enum Dimension { HEIGHT = 6, WIDTH = 10 };

enum coordinate { abscisse = 0, ordonnee = 1 }; 

#define NB_GHOSTS 3
#define NB_FRUIT 20

#define SYMBOL_PACMAN 'C'
#define SYMBOL_GHOST 'F'
#define SYMBOL_FRUIT '*'
#define SYMBOL_FREE ' '
#define SYMBOL_FAIL 'X'

#define CODE_UP 122
#define CODE_DOWN 115
#define CODE_LEFT 113
#define CODE_RIGHT 100
#define CODE_ENTER 10
#define CODE_END_GAME 102

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET "\x1b[0m"

#define PORT 6000
#define MAX_BUFFER 4096

//Structures de données

typedef struct
{
    int x;
    int y;
}Coord;

typedef struct
{
    Coord position;
    int nb_points;
    int nb_fruits;
    bool estVivant;
}PacMan;

typedef struct
{
    Coord position;
    char oldElement;
}Ghost;

// Variables globales
PacMan player = {
        {
                .x = 0,
                .y = 0,
        },
        .estVivant = true,
        .nb_points = 0,
        .nb_fruits = 0,
};

Ghost listGhosts[NB_GHOSTS];

char gameGrid[HEIGHT][WIDTH];

char buffer[MAX_BUFFER];
char tampon[MAX_BUFFER];
#define PORT 6000
#define MAX_CLIENTS 2

const char *EXIT = "exit";


void lireMessage(char tampon[]) {
    printf("Saisir un message à envoyer :\n");
    fgets(tampon, MAX_BUFFER, stdin);
    strtok(tampon, "\n");
}

int testQuitter(char tampon[]) {
    return strcmp(tampon, EXIT) == 0;
}


//Dessine Pacman dans le buffer
void drawPacman() {
    strcat(buffer, COLOR_YELLOW);
    strcat(buffer, " C ");
    strcat(buffer, COLOR_RESET);
    strcat(buffer, "|");
}

//Dessine un fantôme dans le buffer
void drawGhost() {
    strcat(buffer, COLOR_RED);
    strcat(buffer, " F ");
    strcat(buffer, COLOR_RESET);
    strcat(buffer, "|");
}

//Dessine un fruit dans le buffer
void drawFruit() {
    strcat(buffer, COLOR_GREEN);
    strcat(buffer, " * ");
    strcat(buffer, COLOR_RESET);
    strcat(buffer, "|");
}

//Dessine une case vide dans le buffer
void drawBox() {
    
    strcat(buffer, " ");
    strcat(buffer, " ");
    strcat(buffer, " |");
}

void drawFail(){
    strcat(buffer, "");
    strcat(buffer, " X ");
    strcat(buffer, "|");
}

/**
 * Dessine une ligne dans le buffer
 */
void createLine(){
    strcat(buffer, "\n");
    strcat(buffer, "+");
    for(int i = 0; i < WIDTH; ++i){
        strcat(buffer,"---+");
    }
    strcat(buffer, "\n");
}

void drawGrid(){
    sprintf(buffer, "\nScore : %d\n\n", player.nb_points);
    createLine();
    for(int i = 0; i < HEIGHT; ++i){
        strcat(buffer, "|");
        for(int j = 0; j < WIDTH; ++j){
            switch (gameGrid[i][j])
            {
                case SYMBOL_PACMAN  : drawPacman();                     break;
                case SYMBOL_GHOST   : drawGhost();                      break;
                case SYMBOL_FRUIT   : drawFruit();                      break;
                case SYMBOL_FREE    : drawBox();                        break;
                case SYMBOL_FAIL    : drawFail();                        break;
            }
        }
        createLine();
    }
    strcat(buffer, "\n");
}

/**
 * Renvoie le nombre de cases vides dans la grille
 * @return { Integer } numberOfFreeSpaces
 */
int numberOfFreeSpaces(){
    int i,j,numberOfFreeSpaces = 0;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if(gameGrid[i][j] != SYMBOL_GHOST && gameGrid[i][j] != SYMBOL_PACMAN && gameGrid[i][j] != SYMBOL_FRUIT){
                numberOfFreeSpaces +=1;
            }
        }

    }
    return numberOfFreeSpaces;
}

/**
* Renvoie une valeur aléatoire entre [0-maximum]
* @param { Integer } maximum
* @return { Integer } valeur
*/
int randomNumber(int maximum){

    int valeur = rand() % maximum;
    return valeur;
}

/**
 * Création d'un fantôme à une position différente du joueur
 * @return { Ghost } ghost
 */
Ghost createGhost(){
    int x,y;
    do
    {
        x = randomNumber(HEIGHT);
        y = randomNumber(WIDTH);
    } while ((gameGrid[x][y] == SYMBOL_GHOST) || (((x == player.position.x) && (y == player.position.y))));

    Ghost ghost = {
            {
                    .x = x,
                    .y = y,
            },
            .oldElement = SYMBOL_FREE
    };
    return ghost;
}

/**
 * Créer un fruit à une position différente d'un Fantôme et du Joueur
 * @return { Coord } fruit
 */
Coord createFruit(){
    int x,y;
    do
    {
        x = randomNumber(HEIGHT);
        y = randomNumber(WIDTH);
    } while ((gameGrid[x][y] == SYMBOL_GHOST) || (gameGrid[x][y] == SYMBOL_FRUIT) || (((x == player.position.x) && (y == player.position.y))));

    Coord fruit = {
            .x = x,
            .y = y,
    };
    return fruit;
}



/**
 * Renvoie -1 / +1 en fonction du code de la direction
 * @param { Integer } direction
 * @return { Integer } -1 / +1
 */
int giveMoveValue(int direction){

    switch (direction)
    {
    case CODE_UP:
        return -1;
        break;
    case CODE_DOWN:
        return 1;
        break;
    case CODE_LEFT:
        return -1;
        break;
    case CODE_RIGHT:
        return 1;
        break;
    default:
        return EXIT_FAILURE;
        break;
    }
}
/**
 * Renvoie la nouvelle position en abscisse/ordonnee du joueur
 * @param { Integer } coordinate
 * @param { Integer } maximum
 * @return { Integer } result
 */
int calculatePos(int coordinate, int maximum){
    int result = coordinate%maximum;
    if (result < 0)
    {
        result += maximum;
    }
    return result;
}

void refreshPlayer(char symbol){
    if (symbol == SYMBOL_GHOST)
    {
        player.estVivant = false;
        printf("TU AS PERDU !! partie terminee \n");
    }
    if(symbol == SYMBOL_FRUIT){
        player.nb_points += 100;
        player.nb_fruits +=1;
    }
    
}

/**
 * Deplace le joueur dans la grille
 */
void movePlayer(int direction){

    int deplacement = giveMoveValue(direction) ;

    gameGrid[player.position.x][player.position.y] = SYMBOL_FREE;
    
    if (direction == CODE_UP || direction == CODE_DOWN)
    {

        player.position.x = calculatePos((player.position.x + deplacement), HEIGHT);
    }else if(direction == CODE_LEFT || direction == CODE_RIGHT){

        player.position.y = calculatePos((player.position.y + deplacement), WIDTH);

    }

    char element = gameGrid[player.position.x][player.position.y];
    if (element == SYMBOL_GHOST)
    {
        gameGrid[player.position.x][player.position.y] = SYMBOL_FAIL;
    }else{
        gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
    }
    
    refreshPlayer(element);
}


/**
 * Renvoie la nouvelle position en abscisse/ordonnee du fantôme
 * @param { Integer } coordinate
 * @param { Integer } maximum
 * @return { Integer } result
 */
int calculatePosGhost(int coordinate, int maximum){

    // int coordInf = coordinate-1;
    // int coordSup = coordinate+1;
    int tab[2], coordInf = (coordinate-1), coordSup = (coordinate+1);

    if (coordInf >= 0)
    {
        tab[0] = coordInf;
    }else{
        return coordSup;
    }

    if (coordSup < maximum)
    {
        tab[1] = coordSup;
    }else{
        return coordInf;
    }

    int choice = rand()%2;
    return tab[choice];
}

int searchGhost(int x, int y){

    for (int i = 0; i < NB_GHOSTS; i++)
    {
        Ghost ghost = listGhosts[i];
        if ((listGhosts[i].position.x == x) && (listGhosts[i].position.y == y) && (listGhosts[i].oldElement != SYMBOL_GHOST))
        {
            return i;
        }
    }
}

Ghost placeGhostInGrid(Ghost ghost, int indiceGhost){

    if (gameGrid[ghost.position.x][ghost.position.y] == SYMBOL_PACMAN)
    {
        gameGrid[ghost.position.x][ghost.position.y] = SYMBOL_FAIL;
        player.estVivant = false;
    }
    else if(gameGrid[ghost.position.x][ghost.position.y] == SYMBOL_GHOST){
        ghost.oldElement = listGhosts[indiceGhost].oldElement;
        Ghost oldGhost = listGhosts[indiceGhost];
        oldGhost.oldElement = SYMBOL_GHOST;
        listGhosts[indiceGhost] = oldGhost;
    }
    else{
        ghost.oldElement = gameGrid[ghost.position.x][ghost.position.y];
        gameGrid[ghost.position.x][ghost.position.y] = SYMBOL_GHOST;
    }
    return ghost;
}

int getNewPosition(int position, int maximum, int playerPosition){
    int newPosition;
    if (position < playerPosition)
    {
        if (position < maximum)
        {
            newPosition = position + 1;
        }else{
            newPosition = position;
        }
        
    }else{
        if (position > 0)
        {
            newPosition = position - 1;
        }else{
            newPosition = position;
        }
        
    }
    return newPosition;
}


Ghost bestMoveGhost(Ghost ghost){

    if (!player.estVivant)
    {
        return ghost;
    }

    int indiceGhost;
    int choice = rand()%2;

    gameGrid[ghost.position.x][ghost.position.y] = ghost.oldElement;
    
    if ( choice == abscisse )
    {
        ghost.position.x = getNewPosition(ghost.position.x, HEIGHT, player.position.x);
    }

    if( choice == ordonnee ){
        ghost.position.y = getNewPosition(ghost.position.y, WIDTH, player.position.y);
    }

    if (gameGrid[ghost.position.x][ghost.position.y] == SYMBOL_GHOST)
    {
        indiceGhost = searchGhost(ghost.position.x, ghost.position.y);
    }
    
    // Test si on tombe sur le joueur
    ghost = placeGhostInGrid(ghost, indiceGhost);
    return ghost;
}


Ghost moveGhost(Ghost ghost){

    if (!player.estVivant)
    {
        return ghost;
    }

    int choice = rand()%2;
    int indiceGhost;

    gameGrid[ghost.position.x][ghost.position.y] = ghost.oldElement;
    if (choice == abscisse)
    {
        ghost.position.x = calculatePosGhost(ghost.position.x, HEIGHT);
    }

    if(choice == ordonnee){  
        ghost.position.y = calculatePosGhost(ghost.position.y, WIDTH);
    }
        
    if (gameGrid[ghost.position.x][ghost.position.y] == SYMBOL_GHOST)
    {
        indiceGhost = searchGhost(ghost.position.x, ghost.position.y);
    }        

    ghost = placeGhostInGrid(ghost, indiceGhost);

    return ghost;
}

void moveAllGhosts(){
    // Parcours des fantômes
    for (int i = 0; i < NB_GHOSTS; i++)
    {
        int choice = rand()%2;
        
        if (choice == 0)
        {
            listGhosts[i] = bestMoveGhost(listGhosts[i]);
        }else{
            listGhosts[i] = moveGhost(listGhosts[i]);
        }
    }
}

void makeFreeSpaces(){
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            gameGrid[i][j] = SYMBOL_FREE;
        }

    }
}

/**
 * Initialise la grille
 */
void initGrid(){
    // Positionnement des cases vides
    makeFreeSpaces();

    // Positionnement du joueur
    gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;

    // Positionnement des fantômes
    for (int i = 0; i < NB_GHOSTS; ++i) {

        if (numberOfFreeSpaces() == 0)
        {
            break;
        }
        
        Ghost ghost = createGhost();
        
        listGhosts[i] = ghost;

        gameGrid[ghost.position.x][ghost.position.y] = SYMBOL_GHOST;
    }

    // Positionnement des fruits
    for (int i = 0; i < NB_FRUIT; ++i) {
        if (numberOfFreeSpaces() == 0)
        {
            break;
        }

        Coord fruit = createFruit();

        gameGrid[fruit.x][fruit.y] = SYMBOL_FRUIT;
    }


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

//Création de la structure d'adresse
struct sockaddr_in initAddr(int type){
    struct sockaddr_in coord;
    int longueurAdresse = sizeof(struct sockaddr_in);
    memset(&coord, 0x00, longueurAdresse);
    coord.sin_family = PF_INET;
    coord.sin_port = htons(PORT);
    if(type==1){
        coord.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else if(type==2){
        inet_aton("127.0.0.1", &coord.sin_addr);
    }
    return coord;
}

//Mise en écoute du serveur
int listenSocket(){
    int fdSocket = initSocket();
    struct sockaddr_in coordonneesServeur = initAddr(1);

    if (bind(fdSocket, (struct sockaddr *) &coordonneesServeur, sizeof(coordonneesServeur)) == -1) {
        printf("Erreur de nommage \n");
        exit(EXIT_FAILURE);
    }
    if (listen(fdSocket, MAX_CLIENTS) == -1) {
        printf("Erreur de listen \n");
        exit(EXIT_FAILURE);
    }

    return fdSocket;
}

//Gestion de la connexion client
int clientConnection(int fdSocketServer){
    struct sockaddr_in coordClient = initAddr(2);
    socklen_t coordSize = sizeof(coordClient);
    int fdSocketClient = accept(fdSocketServer, (struct sockaddr *) &coordClient, &coordSize);

    if(fdSocketClient == -1) {
        printf("Erreur de connexion avec le client\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Connexion du client %s:%d acceptée\n", inet_ntoa(coordClient.sin_addr), ntohs(coordClient.sin_port));
    }

    return fdSocketClient;
}


int main(int argc, char const *argv[]) {
    int fdSocketAttente;
    int fdSocketCommunication;
    int nbRecu;
    int longueurAdresse;
    int pid;

    fdSocketAttente = listenSocket();

    printf("En attente de connexion des clients\n");

    

    int nbClients = 0;

    while (nbClients < MAX_CLIENTS) {
        
        fdSocketCommunication = clientConnection(fdSocketAttente);
        
        // if ((fdSocketCommunication = accept(fdSocketAttente, (struct sockaddr *) &coordonneesAppelant,
        //                                     &tailleCoord)) == -1) {
        //     printf("erreur de accept\n");
        //     exit(EXIT_FAILURE);
        // }else{
        //     nbClients++;
        // }

        if (fdSocketCommunication > 0)
        {
            nbClients++;
        }
        

        if ((pid = fork()) == 0) {
            initGrid();
            int direction;
            while (1) {

                do
                {
                    drawGrid();
                    
                    send(fdSocketCommunication, buffer, strlen(buffer), 0);

                    if(player.estVivant){
                        // on attend le message du client
                        nbRecu = recv(fdSocketCommunication, tampon, MAX_BUFFER, 0);

                        if (nbRecu > 0) {
                            tampon[nbRecu] = 0;
                            direction = tampon[0];
                            if (direction == CODE_END_GAME)
                            {
                                printf("Un client a mis fin à la partie \n");
                                player.estVivant = false;
                            }
                            
                        }
                        movePlayer(direction);
                        moveAllGhosts();
                    }
                    // envoie d'une variable qui met fin à la partie
                    // envoyer un message qui annonce la fin de la partie

                } while (player.estVivant && (player.nb_fruits < NB_FRUIT));

                
            }

            exit(EXIT_SUCCESS);
            close(fdSocketCommunication);
        }

    }

    // close(fdSocketCommunication);
    close(fdSocketAttente);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        wait(NULL);
    }

    printf("Fin du programme.\n");
    return EXIT_SUCCESS;
}
