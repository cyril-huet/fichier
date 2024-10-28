#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "image.h"
#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Initialiser la grille
    Grid grid;
    create_grid(&grid, 10, 10); // Exemple: 10x10
    fill_grid(&grid);
    load_grid_image("src/grid_image.png", &grid);
    // Découper et enregistrer les images
    save_char_images(&grid, "images/");

    // Libérer la mémoire
    free_grid(&grid);
    
    return 0;
}

