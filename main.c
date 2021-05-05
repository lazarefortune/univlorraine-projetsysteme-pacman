#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define Height 6
#define Width 10
#define NB_GHOSTS 5

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

struct PacMan playerPacMan = {
        {
            .x = 0,
            .y = 0,
        },
        .nb_point = 0
};

void initialize()
{
    // initialisation de la grille.
    
    // placer tous les fantômes de façon aléatoire.

    // placer les points à ramasser

    // placer PACMAN
}

char gameGrid[Height][Width];

bool ifEmpty(){
    //for (int x = 0; x < Height; x++)
    //{
        for (int y = 0; y < Width; y++)
        {
            //printf("x = %d, y = %d ,value = %s",x,y, gameGrid[x][y]);
            //printf("\n");
            printf("%d \n", y);
           /* if (!gameGrid[x][y])
            {
                printf("c_est vide \n");
            }*/
        }
      //  break;
    //}
    //return false;
    
}

void initialize_grid(){
    // Initialisation des fantômes
    for (int i = 0; i < NB_GHOSTS; ++i) {
        srand(time(NULL));
        int x = rand() % Height;
        int y = rand() % Width;
        // On vérifie si la position n'est pas prise
         ifEmpty();
         /*
        printf("un nombre = %d \n", rand());
        printf("un nombre 2 = %d \n\n", rand());
        printf("x = %d et y = %d pour le fantome %d \n\n", x,y,i);
        gameGrid[x][y] = putchar('F');
        struct Ghost ghost = {
                {
                        .x = x,
                        .y = y,
                }
        };
        listGhosts[i] = ghost;
        */
    }
/*
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            if(gameGrid[i][j] != 'F'){
                gameGrid[i][j] = putchar(' ');
            }
        }
        
    }
    */
}

// Affiche la grille entière
void display_grid(){
    printf("\n");
    putchar('+');

    for(int i = 0; i < Width; ++i){
        printf("---+");
    }

    putchar('\n');

    for(int i = 0; i < Height; ++i){
        putchar('|');

        for(int j = 0; j < Width; ++j){

            printf(" %c |", gameGrid[i][j]);
        }

        putchar('\n');
        putchar('+');

        for(int j = 0; j < Width; ++j){
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
//    initialize_grid();
    //display_grid();    

    return 0;
}
