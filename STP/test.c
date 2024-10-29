#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define CELL_WIDTH 50    // Largeur d'une cellule dans la grille
#define CELL_HEIGHT 50   // Hauteur d'une cellule dans la grille
#define GRID_ROWS 10     // Nombre de lignes dans la grille
#define GRID_COLS 10     // Nombre de colonnes dans la grille

void save_character(SDL_Surface *image, int row, int col, const char *output_folder) {
    // Définir le rectangle de découpage pour chaque lettre
    SDL_Rect src_rect;
    src_rect.x = col * CELL_WIDTH;
    src_rect.y = row * CELL_HEIGHT;
    src_rect.w = CELL_WIDTH;
    src_rect.h = CELL_HEIGHT;

    // Créer une surface pour l'image du caractère
    SDL_Surface *character_surface = SDL_CreateRGBSurface(0, CELL_WIDTH, CELL_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (character_surface == NULL) {
        fprintf(stderr, "Erreur lors de la création de la surface du caractère : %s\n", SDL_GetError());
        return;
    }

    // Copier la portion de la grille dans la surface de caractère
    SDL_BlitSurface(image, &src_rect, character_surface, NULL);

    // Créer le nom de fichier pour chaque caractère
    char filename[100];
    snprintf(filename, sizeof(filename), "%s/char_%d_%d.png", output_folder, row, col);

    // Sauvegarder l'image du caractère
    if (IMG_SavePNG(character_surface, filename) != 0) {
        fprintf(stderr, "Erreur lors de la sauvegarde de l'image du caractère : %s\n", IMG_GetError());
    }

    // Libérer la surface du caractère
    SDL_FreeSurface(character_surface);
}

void segment_characters(const char *image_path, const char *output_folder) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return;
    }

    // Charger l'image source
    SDL_Surface *image = IMG_Load(image_path);
    if (image == NULL) {
        fprintf(stderr, "Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        SDL_Quit();
        return;
    }

    // Parcourir chaque cellule de la grille et extraire chaque caractère
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            save_character(image, row, col, output_folder);
        }
    }

    // Libérer l'image source et quitter SDL
    SDL_FreeSurface(image);
    SDL_Quit();
}

int main() {
    const char *image_path = "/grid_only.png";  // Chemin de l'image source
    const char *output_folder = "characters";            // Dossier de sortie pour les images de caractères

    segment_characters(image_path, output_folder);

    return 0;
}

