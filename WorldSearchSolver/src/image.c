#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

void save_char_images(Grid *grid, const char *folder) {
    SDL_Init(SDL_INIT_VIDEO);

    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            char filename[100];
            snprintf(filename, sizeof(filename), "%schar_%d_%d.png", folder, i, j);
            
            // Créer une surface pour le caractère
            SDL_Surface *surface = SDL_CreateRGBSurface(0, 50, 50, 32, 0, 0, 0, 0);
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255)); // Fond blanc

            // Dessiner le caractère sur la surface
            SDL_Surface *text_surface = TTF_RenderText_Solid(font, (char[]){grid->characters[i][j], '\0'}, textColor);
            SDL_BlitSurface(text_surface, NULL, surface, NULL);
            SDL_SavePNG(surface, filename); // Sauvegarder l'image
            
            SDL_FreeSurface(text_surface);
            SDL_FreeSurface(surface);
        }
    }

    SDL_Quit();
}

