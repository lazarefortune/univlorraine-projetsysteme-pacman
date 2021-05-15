#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 4
#define WIDTH 4

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
#define CODE_END 102

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
    char direction, clavier;
    int code;
    do{
        if (code != CODE_ENTER)
        {
            printf("z(haut),q(gauche),d(droite),s(bas), f(Fin du jeu)\n");
        }

        scanf("%c", &direction);
        code = (int)direction;
        // printf("code : %d \n", code);
        condition = ((code != CODE_UP ) && (code != CODE_LEFT) && (code !=  CODE_DOWN) && (code != CODE_RIGHT) && (code != CODE_ENTER) && (code != CODE_END));
        showError("Erreur de saisie", condition);
    }while(condition);

    switch(direction) {
        case CODE_UP: printf("Fleche du haut\n");        break;
        case CODE_DOWN: printf("Fleche du bas\n");         break;
        case CODE_LEFT: printf("Fleche de gauche\n");      break;
        case CODE_RIGHT: printf("Fleche de droite\n");      break;
        case CODE_END: printf("Fin du jeu...\n");           break;
        case CODE_ENTER:                                break;
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
            player.nb_fruits +=1;

        }else if(element == SYMBOL_GHOST){

            gameGrid[oldValue][player.position.y] = SYMBOL_FREE;
            gameGrid[player.position.x][player.position.y] = SYMBOL_FAIL;
            player.estVivant = false;
            printf("TU AS PERDU !! partie terminee \n");

        }else if (element == SYMBOL_FREE){

            gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
            gameGrid[oldValue][player.position.y] = SYMBOL_FREE;

        }
    }else if(direction == CODE_LEFT || direction == CODE_RIGHT){
        oldValue = player.position.y;
        player.position.y = calculatePos((player.position.y + deplacement), WIDTH);

        // on test ce qui se trouve à cet endroit dans la grille
        char element = gameGrid[player.position.x][player.position.y];
        if (element == SYMBOL_FRUIT)
        {

            gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
            gameGrid[player.position.x][oldValue] = SYMBOL_FREE;
            player.nb_point += 100;
            player.nb_fruits +=1;

        }else if(element == SYMBOL_GHOST){

            gameGrid[player.position.x][player.position.y] = SYMBOL_FAIL;
            gameGrid[player.position.x][oldValue] = SYMBOL_FREE;
            player.estVivant = false;
            printf("TU AS PERDU !! partie terminee \n");

        }else if (element == SYMBOL_FREE){

            gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
            gameGrid[player.position.x][oldValue] = SYMBOL_FREE;

        }
    }
}


/**
 * Renvoie la nouvelle position en abscisse/ordonnee du fantôme
 * @param { Integer } coordonnee
 * @param { Integer } maximum
 * @return { Integer } resultat
 */
int calculatePosGhost(int coordonnee, int maximum){

    // int coordInf = coordonnee-1;
    // int coordSup = coordonnee+1;
    int tab[2], coordInf = (coordonnee-1), coordSup = (coordonnee+1);

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

    int choix = rand()%2;
    return tab[choix];
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
    printf("\n");
}

Ghost moveGhost(Ghost ghost){

    if (!player.estVivant)
    {
        return ghost;
    }

    // choix de la coordonnee à modifier
    int choix = rand()%2, oldValueCoord, newValueCoord;
    int indiceGhost;

    if (choix == 0)
    {
        newValueCoord = calculatePosGhost(ghost.position.x, HEIGHT);

        oldValueCoord = ghost.position.x;
        // s'il y a un fantôme à cet endroit on ne fait rien
        if (gameGrid[newValueCoord][ghost.position.y] == SYMBOL_GHOST)
        {
            indiceGhost = searchGhost(newValueCoord, ghost.position.y);
        }

        // On modifie les coordonnees du ghost en x
        ghost.position.x = newValueCoord;

        // On place l'élément présent avant le passage du Ghost
        gameGrid[oldValueCoord][ghost.position.y] = ghost.oldElement;
    }
    if(choix == 1){
        newValueCoord = calculatePosGhost(ghost.position.y, WIDTH);

        oldValueCoord = ghost.position.y;
        // s'il y a un fantôme à cet endroit on ne fait rien
        if (gameGrid[ghost.position.x][newValueCoord] == SYMBOL_GHOST)
        {
            indiceGhost = searchGhost(ghost.position.x, newValueCoord);
        }

        // On modifie les coordonnees du ghost en y
        ghost.position.y = newValueCoord;

        // On place l'élément présent avant le passage du Ghost
        gameGrid[ghost.position.x][oldValueCoord] = ghost.oldElement;
    }

    // Test si on tombe sur le joueur
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

void moveAllGhosts(){
    // Parcours des fantômes
    for (int i = 0; i < NB_GHOSTS; i++)
    {
        // On déplace un fantôme
        listGhosts[i] = moveGhost(listGhosts[i]);
    }
}

// ******************************************** //

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    initialize_grid();
    printf("\n");
    display_grid();

    int nbFruits;
    bool vivant;
    do
    {
        int direction = chooseDirection();
        if (direction != CODE_ENTER)
        {
            if(direction == CODE_END){
                player.estVivant = false;
            }else{
                movePlayer(direction);
                moveAllGhosts();
                display_grid();
                nbFruits = player.nb_fruits;
                printf("\n Il y a %d fruits collectés \n", nbFruits);
            }
            printf("Votre score : %d \n", player.nb_point);
        }
    } while (player.estVivant && (player.nb_fruits < NB_FRUIT));

    int som = numberOfFreeSpaces();
    printf("\n Il y a %d cases vides \n", som);

    return 0;
}
