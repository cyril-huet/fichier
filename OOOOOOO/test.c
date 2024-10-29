#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#define CHAR_WIDTH 50  // À ajuster selon la largeur d'une cellule de la grille
#define CHAR_HEIGHT 50  // À ajuster selon la hauteur d'une cellule de la grille

int is_black_pixel(SDL_Surface *surface, int x, int y, int seuil) {
    Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    return (r < seuil && g < seuil && b < seuil);
}

void save_character(SDL_Surface *surface, int left, int top, int charWidth, int charHeight, int index) {
    SDL_Surface *charSurface = SDL_CreateRGBSurface(0, charWidth, charHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!charSurface) {
        fprintf(stderr, "Erreur de création de surface pour caractère: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect srcRect = { left, top, charWidth, charHeight };
    SDL_Rect destRect = { 0, 0, charWidth, charHeight };
    SDL_BlitSurface(surface, &srcRect, charSurface, &destRect);

    char filename[100];
    sprintf(filename, "character_%d.bmp", index);
    SDL_SaveBMP(charSurface, filename);
    SDL_FreeSurface(charSurface);
}

void segment_characters_grid(SDL_Surface *surface) {
    int index = 0;
    int seuil = 50;  // Tolérance pour détecter les pixels "noirs"
    for (int row = 0; row < surface->h / CHAR_HEIGHT; row++) {
        for (int col = 0; col < surface->w / CHAR_WIDTH; col++) {
            int left = col * CHAR_WIDTH;
            int top = row * CHAR_HEIGHT;
            
            // Vérifie s'il y a un caractère dans cette cellule
            int has_char = 0;
            for (int y = top; y < top + CHAR_HEIGHT && y < surface->h; y++) {
                for (int x = left; x < left + CHAR_WIDTH && x < surface->w; x++) {
                    if (is_black_pixel(surface, x, y, seuil)) {
                        has_char = 1;
                        break;
                    }
                }
                if (has_char) break;
            }

            // Sauvegarde le caractère s'il est présent
            if (has_char) {
                save_character(surface, left, top, CHAR_WIDTH, CHAR_HEIGHT, index);
                index++;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface *imageSurface = SDL_LoadBMP("binarized_wordsearch.bmp");
    if (!imageSurface) {
        fprintf(stderr, "Erreur lors du chargement de l'image: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    segment_characters_grid(imageSurface);

    SDL_FreeSurface(imageSurface);
    SDL_Quit();

    return 0;
}

