#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 6
#define WIDTH 10

#define NB_GHOSTS 4
#define NB_FRUIT 20

// Le nombre de points qui met fin à la partie
#define POINT_FAIL -1

#define SYMBOL_PACMAN 'C'
#define SYMBOL_GHOST 'F'
#define SYMBOL_FRUIT '*'
#define SYMBOL_FREE ' '
#define SYMBOL_FAIL 'X'

#define CODE_UP 122
#define CODE_DOWN 115
#define CODE_LEFT 113
#define CODE_RIGHT 100

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET "\x1b[0m"

struct coord
{
    int x;
    int y;
};

struct PacMan
{
    struct coord position;
    int nb_point;
};

struct Ghost
{
    struct coord position;
    char oldElement;
};

struct Ghost listGhosts[NB_GHOSTS];

struct PacMan player = {
        {
            .x = 2,
            .y = 5,
        },
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
 * Initialise la grille
 */
void initialize_grid(){

    // Positionnement des fantômes
    for (int i = 0; i < NB_GHOSTS; ++i) {
        if (numberOfFreeSpaces() == 0)
        {
            printf("Il n'y a plus de cases vides");
            break;
        }
        int x,y;
        do
        {
            x = randomNumber(HEIGHT);
            y = randomNumber(WIDTH);
        } while ((gameGrid[x][y] == SYMBOL_GHOST) || (((x == player.position.x) && (y == player.position.y))));

        struct Ghost ghost = {
                {
                        .x = x,
                        .y = y,
                }
        };

        listGhosts[i] = ghost;

        gameGrid[x][y] = SYMBOL_GHOST;
    }

    // Positionnement du joueur
    gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;

    // Positionnement des fruits
    for (int i = 0; i < NB_FRUIT; ++i) {
        if (numberOfFreeSpaces() == 0)
        {
            printf("Il n'y a plus de cases vides");
            break;
        }

        int x,y;
        do
        {
            x = randomNumber(HEIGHT);
            y = randomNumber(WIDTH);
        } while ((gameGrid[x][y] == SYMBOL_GHOST) || (gameGrid[x][y] == SYMBOL_FRUIT) || (((x == player.position.x) && (y == player.position.y))));
    
        gameGrid[x][y] = SYMBOL_FRUIT;
    }

    
    // Positionnement des cases vides
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if(gameGrid[i][j] != SYMBOL_GHOST && gameGrid[i][j] != SYMBOL_PACMAN && gameGrid[i][j] != SYMBOL_FRUIT){
                gameGrid[i][j] = ' ';
            }
        }

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
    int direction;
    
    do{
        printf("Choisissez une direction: z(haut),q(gauche),d(droite),s(bas) du clavier\n");
        direction = getchar();
        condition = ((direction != CODE_UP ) && (direction != CODE_LEFT) && (direction !=  CODE_DOWN) && (direction != CODE_RIGHT));
        showError("Erreur de saisie", condition);
    }while(condition);

    switch(direction) {
        case CODE_UP: printf("Fleche du haut\n");        break;
        case CODE_DOWN: printf("Fleche du bas\n");         break;
        case CODE_LEFT: printf("Fleche de gauche\n");      break;
        case CODE_RIGHT: printf("Fleche de droite\n");      break;
        default: printf("touche non reconnue");      break;
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
 * @param { Integer } coordonnee
 * @param { Integer } maximum
 * @return { Integer } resultat
 */
int calculatePos(int coordonnee, int maximum){
    int resultat = coordonnee%maximum;
    if (resultat < 0)
    {
        resultat += maximum;
    }
    return resultat;
}

/**
 * Deplace le joueur dans la grille
 */
void movePlayer(int direction){

    /*
    [
        0  [0,1,2,3,4,5,6],
        1  [0,1,2,3,4,5,6],
        2  [0,1,2,3,4,5,6],
        3  [0,1,2,3,4,5,6],
    ]
    */

    int oldValue = 0;
    int deplacement = giveMoveValue(direction) ;
    // A REFACTORER ---- TROP LONG
    if (direction == CODE_UP || direction == CODE_DOWN)
    {
        oldValue = player.position.x;
        // player.position.x = (player.position.x + deplacement)%HEIGHT;
        player.position.x = calculatePos((player.position.x + deplacement), HEIGHT);

        // on test ce qui se trouve à cet endroit dans la grille
        char element = gameGrid[player.position.x][player.position.y];
        if (element == SYMBOL_FRUIT)
        {
            gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
            gameGrid[oldValue][player.position.y] = SYMBOL_FREE;
            player.nb_point += 100;
            // printf("le joueur possède %d points\n", player.nb_point);

        }else if(element == SYMBOL_GHOST){

            gameGrid[oldValue][player.position.y] = SYMBOL_FREE;
            gameGrid[player.position.x][player.position.y] = SYMBOL_FAIL;
            player.nb_point = POINT_FAIL;
            printf("TU AS PERDU !! partie terminee \n");

        }else if (element == SYMBOL_FREE){

            gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
            gameGrid[oldValue][player.position.y] = SYMBOL_FREE;

        }
    }else if(direction == CODE_LEFT || direction == CODE_RIGHT){
        oldValue = player.position.y;
        // player.position.y = (player.position.y + deplacement)%WIDTH;
        player.position.y = calculatePos((player.position.y + deplacement), WIDTH);

        // on test ce qui se trouve à cet endroit dans la grille
        char element = gameGrid[player.position.x][player.position.y];
        if (element == SYMBOL_FRUIT)
        {

            gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
            gameGrid[player.position.x][oldValue] = SYMBOL_FREE;
            player.nb_point += 100;
            // printf("le joueur possède %d points\n", player.nb_point);

        }else if(element == SYMBOL_GHOST){

            gameGrid[player.position.x][player.position.y] = SYMBOL_FAIL;
            gameGrid[player.position.x][oldValue] = SYMBOL_FREE;
            player.nb_point = POINT_FAIL;
            printf("TU AS PERDU !! partie terminee \n");

        }else if (element == SYMBOL_FREE){

            gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
            gameGrid[player.position.x][oldValue] = SYMBOL_FREE;

        }
    }

    display_grid();
    // printf("le joueur possède %d points\n", player.nb_point);      
}

// ******************************************** //

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    initialize_grid();
    printf("\n");
    display_grid();
    movePlayer(CODE_LEFT);
    movePlayer(CODE_UP);
    movePlayer(CODE_LEFT);
    movePlayer(CODE_DOWN);
    movePlayer(CODE_RIGHT);
    int som = numberOfFreeSpaces();
    printf("\n Il y a %d cases vides \n", som);

    return 0;
}
