#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grid.h"

void create_grid(Grid *grid, int rows, int cols) {
    grid->rows = rows;
    grid->cols = cols;
    grid->characters = malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        grid->characters[i] = malloc(cols * sizeof(char));
    }
}

void fill_grid(Grid *grid) {
    // Exemple de remplissage de la grille avec des caractères aléatoires
    const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            grid->characters[i][j] = alphabet[rand() % 26];
        }
    }
}

void free_grid(Grid *grid) {
    for (int i = 0; i < grid->rows; i++) {
        free(grid->characters[i]);
    }
    free(grid->characters);
}

