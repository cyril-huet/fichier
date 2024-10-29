#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define CELL_SIZE 50  // Taille approximative de chaque case, ajustable
#define GRID_ROWS 8   // Nombre de lignes dans la grille
#define GRID_COLS 8   // Nombre de colonnes dans la grille

void saveLetter(SDL_Surface* source, int x, int y, int index) {
    // Définir la zone de découpe
    SDL_Rect srcRect = { x, y, CELL_SIZE, CELL_SIZE };
    // Créer une surface pour l'image de la lettre
    SDL_Surface* letterSurface = SDL_CreateRGBSurface(0, CELL_SIZE, CELL_SIZE, 32, 0, 0, 0, 0);
    // Copier la lettre de la surface source vers la surface de la lettre
    SDL_BlitSurface(source, &srcRect, letterSurface, NULL);
    
    // Générer un nom de fichier unique pour chaque lettre
    char filename[20];
    snprintf(filename, sizeof(filename), "letter_%d.png", index);
    
    // Sauvegarder l'image de la lettre
    IMG_SavePNG(letterSurface, filename);
    SDL_FreeSurface(letterSurface);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    IMG_Init(IMG_INIT_PNG);

    // Charger l'image binarisée
    SDL_Surface* image = IMG_Load("grid_only.png");
    if (!image) {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }

    // Parcourir chaque case de la grille et sauvegarder chaque lettre
    int index = 0;
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            int x = col * CELL_SIZE;
            int y = row * CELL_SIZE;
            saveLetter(image, x, y, index);
            index++;
        }
    }

    // Libérer les ressources
    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

