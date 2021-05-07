#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>

//colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

//grid enabled sizes
#define SM_GRID_WIDTH 10
#define SM_GRID_HEIGHT 6
#define MD_GRID_WIDTH 13
#define MD_GRID_HEIGHT 9
#define LG_GRID_WIDTH 16
#define LG_GRID_HEIGHT 12

//grids
char** sm_grid[SM_GRID_WIDTH][SM_GRID_HEIGHT];
char** md_grid[MD_GRID_WIDTH][MD_GRID_HEIGHT];
char** lg_grid[LG_GRID_WIDTH][LG_GRID_HEIGHT];

// Affiche la grille entière
void display_grid(int col, int line){
    printf("\nTour du joueur : %d\t\t", 0);
    printf("Score : %d\n\n", 0);
    putchar('+');

    for(int i = 0; i < col; ++i){
        printf("---+");
    }

    putchar('\n');

    for(int i = 0; i < line; ++i){
        putchar('|');

        for(int j = 0; j < col; ++j){
            /*if (isalpha(grid[i][j]))
                printf(" %c |", grid[i][j]);
            else*/
            printf(" %c |", ' ');
        }

        putchar('\n');
        putchar('+');

        for(int j = 0; j < col; ++j){
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


int main(){
    char grid_size[10];
    char* sm_grid = "petite";
    char* md_grid = "moyenne";
    char* lg_grid = "grande";
    bool correct_choice = false;

    //User's grid size choice
    
    chooseDirection();

    return 0;
}