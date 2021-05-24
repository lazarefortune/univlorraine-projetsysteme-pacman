#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// #define HEIGHT 10
// #define WIDTH 10

enum Dimension { HEIGHT = 10, WIDTH = 10 };

enum coordinate { abscisse = 0, ordonnee = 1 }; 

#define NB_GHOSTS 5
#define NB_FRUIT 6

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


typedef struct Coord
{
    int x;
    int y;
}Coord;

typedef struct PacMan
{
    Coord position;
    int nb_point;
    int nb_fruits;
    bool estVivant;
}PacMan;

typedef struct Ghost
{
    Coord position;
    char oldElement;
} Ghost;

struct Ghost listGhosts[NB_GHOSTS];

struct PacMan player = {
        {
            .x = 0,
            .y = 0,
        },
        .estVivant = true,
        .nb_point = 0
};


char gameGrid[HEIGHT][WIDTH];

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
 * Parcours la grille et remplie les espaces vides
 */
// void makeFreeSpaces(){
//     for (int i = 0; i < HEIGHT; i++)
//     {
//         for (int j = 0; j < WIDTH; j++)
//         {
//             if(gameGrid[i][j] != SYMBOL_GHOST && gameGrid[i][j] != SYMBOL_PACMAN && gameGrid[i][j] != SYMBOL_FRUIT){
//                 gameGrid[i][j] = SYMBOL_FREE;
//             }
//         }

//     }
// }

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
void initialize_grid(){
    // Positionnement des cases vides
    makeFreeSpaces();

    // Positionnement du joueur
    gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;

    // Positionnement des fantômes
    for (int i = 0; i < NB_GHOSTS; ++i) {

        if (numberOfFreeSpaces() == 0)
        {
            printf("Il n'y a plus de cases vides");
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
            printf("Il n'y a plus de cases vides");
            break;
        }

        Coord fruit = createFruit();

        gameGrid[fruit.x][fruit.y] = SYMBOL_FRUIT;
    }


}

/**
 * Affiche une ligne
 */
void createLine(){
    putchar('\n');
    putchar('+');
    for(int i = 0; i < WIDTH; ++i){
        printf("---+");
    }
    putchar('\n');
}

/**
 * Affiche la grille entière
 */
void display_grid(){

    createLine();

    for(int i = 0; i < HEIGHT; ++i){
        putchar('|');
        for(int j = 0; j < WIDTH; ++j){
            switch (gameGrid[i][j])
            {
                case SYMBOL_PACMAN:
                    printf(COLOR_YELLOW " %c " COLOR_RESET, gameGrid[i][j]);
                    printf("|");
                    break;
                case SYMBOL_GHOST:
                    printf(COLOR_RED " %c " COLOR_RESET, gameGrid[i][j]);
                    printf("|");
                    break;
                case SYMBOL_FRUIT:
                    printf(COLOR_GREEN " %c " COLOR_RESET, gameGrid[i][j]);
                    printf("|");
                    break;
                default:
                    printf( " %c |", gameGrid[i][j]);
                    break;
            }

        }
        createLine();
    }

    printf("\n");
}

/**
 * Affiche un message si la condition est remplie
 * @param { char* } message
 * @param { bool } condition
 */
void showError(char* message,bool condition){

    if (condition)
    {
        printf("%s \n", message);
    }
}

/**
 * Retourne un entier qui correspond à la direction saisie
 * @return { Integer } direction
 */
int chooseDirection(){

    bool condition;
    char direction;

    do{
        printf("z : haut | q : gauche | d : droite | s : bas | f : Fin du jeu\n");
        int res = scanf(" %c", &direction);
        printf("\t direction %c \n", direction);
        
        condition = ((direction != CODE_UP ) && (direction != CODE_LEFT) && (direction !=  CODE_DOWN) && (direction != CODE_RIGHT) && (direction != CODE_END_GAME));
        showError("Erreur de saisie", condition);
    }while(condition);

    if (direction == CODE_END_GAME)
    {
        printf("Fin du jeu... \n");
    }

    return direction;
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
        player.nb_point += 100;
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
        if (position < WIDTH)
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

// ******************************************** //

void runGame(){
    initialize_grid();
    printf("\n");
    display_grid();
    
    do
    {
        int direction = chooseDirection();

        if(direction == CODE_END_GAME){
            player.estVivant = false;
        }else{
            movePlayer(direction);
            moveAllGhosts();
            display_grid();
            printf("\n Il y a %d fruits collectes \n", player.nb_fruits);
        }
        printf("Votre score : %d \n", player.nb_point);

    } while (player.estVivant && (player.nb_fruits < NB_FRUIT));
    // envoyer au client fin de la partie
}

enum menuOptions { play = 49, score = 50 ,end = 51 };

void menu(){
    char choice;

    printf("---------Menu--------\n");
    printf("1. Jouer \n");
    printf("2. Consulter les scores \n");
    printf("3. Quitter le jeu \n"); 
    
    do
    {
        scanf(" %c", &choice);
        // printf("\t avant cast la valeur : %c \n", choice);
        
        printf("la valeur : %d \n", choice);

    } while (choice != play && choice != score && choice != end);
    

    switch (choice)
    {
    case play:
        runGame();
        break;
    case score:
        break;
    default:
        break;
    }


    // int direction = chooseDirection();
    // printf("valeur : %d\n", direction);
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    menu();
    // runGame();
    
    
    return 0;
}
