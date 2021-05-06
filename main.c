#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define HEIGHT 6
#define WIDTH 10
#define NB_GHOSTS 3
#define SYMBOL_PACMAN 'C'
#define SYMBOL_GHOST 'F'
#define SYMBOL_FRUIT '*'

#define COLOR_RED "\x1b[31m"
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
            .x = 0,
            .y = 0,
        },
        .nb_point = 0
};


char gameGrid[HEIGHT][WIDTH];

/*
*   Cette fonction teste s'il existe un Ghost à la position (x,y)
*   Return True, si oui, False sinon
*/
bool ifExistGhost(int x, int y){
    for (int i = 0; i < NB_GHOSTS; i++)
    {
        if(listGhosts[i].position.x == x && listGhosts[i].position.y == y){
            return true;
        }
    }
    return false;
}

void initialize_grid(){
    // Initialisation des fantômes
    for (int i = 0; i < NB_GHOSTS; ++i) {
        // On créé des coordonnées aléatoires
        int x,y;
        do
        {
        //  srand(time(NULL));

            x = rand() % HEIGHT;
            y = rand() % WIDTH;
        } while (ifExistGhost(x,y));
        // On créé un nouveau fantôme
        struct Ghost ghost = {
                {
                        .x = x,
                        .y = y,
                }
        };
        // on l'ajoute à la liste des fantômes
        listGhosts[i] = ghost;
        // printf("x = %d et y = %d pour le fantome %d \n\n", x,y,i);

        // on l'ajoute dans la grille de jeu
        gameGrid[x][y] = SYMBOL_GHOST;

    }
    // On ajoute notre PACMAN
    gameGrid[player.position.x][player.position.y] = SYMBOL_PACMAN;
    // On remplie toute la grille avec les espaces vides
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if(gameGrid[i][j] != SYMBOL_GHOST && gameGrid[i][j] != SYMBOL_PACMAN){
                gameGrid[i][j] = SYMBOL_FRUIT;
            }
        }

    }
}

// Affiche la grille entière
void display_grid(){

    printf("\n");

    putchar('+');
    for(int i = 0; i < WIDTH; ++i){
        printf("---+");
    }

    putchar('\n');

    for(int i = 0; i < HEIGHT; ++i){
        putchar('|');

        for(int j = 0; j < WIDTH; ++j){

            if (gameGrid[i][j] == SYMBOL_PACMAN)
            {
                printf(COLOR_YELLOW " %c " COLOR_RESET, gameGrid[i][j]);
                printf("|");

            }else if(gameGrid[i][j] == SYMBOL_GHOST){

                printf(COLOR_RED " %c " COLOR_RESET, gameGrid[i][j]);
                printf("|");
            }else{

                // printf(COLOR_RED " %c " COLOR_RESET, gameGrid[i][j]);
                printf( " %c |", gameGrid[i][j]);
            }

        }

        putchar('\n');
        putchar('+');

        for(int j = 0; j < WIDTH; ++j){
            printf("---+");
        }

        putchar('\n');
    }

    putchar('\n');
}

// Affiche un message si la condition est remplie
void showError(message, condition){

    if (condition)
    {
        printf("%s \n", message);
    }
}

// Retourne un entier qui correspond à la direction choisie
int chooseDirection(){

    bool condition;
    int direction;
    do{
        printf("Choisissez une direction avec les fleches du clavier\n");
        direction = _getch();
        condition = ((direction != 72 ) && (direction != 75) && (direction !=  77) && (direction != 80));
        showError("Erreur de saisie", condition);
    }while(condition);

    switch(direction) {
        case 72: printf("Fleche du haut\n");        break;
        case 75: printf("Fleche de gauche\n");      break;
        case 77: printf("Fleche de droite\n");      break;
        case 80: printf("Fleche du bas\n");         break;
    }
    return direction;
}

int main(int argc, char const *argv[])
{
    initialize_grid();
    printf("\n");
    display_grid();

    return 0;
}
