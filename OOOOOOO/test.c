#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define THRESHOLD 128 // Seuil pour la binarisation
#define MIN_CHARACTER_SIZE 20 // Taille minimale d'un caractère (en pixels)

void binarize(SDL_Surface *image) {
    for (int y = 0; y < image->h; y++) {
        for (int x = 0; x < image->w; x++) {
            Uint32 pixel = ((Uint32 *)image->pixels)[y * image->w + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            Uint8 gray = (Uint8)(0.3 * r + 0.59 * g + 0.11 * b);

            if (gray < THRESHOLD) {
                ((Uint32 *)image->pixels)[y * image->w + x] = SDL_MapRGB(image->format, 0, 0, 0);
            } else {
                ((Uint32 *)image->pixels)[y * image->w + x] = SDL_MapRGB(image->format, 255, 255, 255);
            }
        }
    }
}

void save_letter(SDL_Surface *image, int x, int y, int width, int height, int letter_index) {
    SDL_Rect rect = {x, y, width, height};
    SDL_Surface *letter_surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_BlitSurface(image, &rect, letter_surface, NULL);

    char filename[50];
    snprintf(filename, sizeof(filename), "letters/letter_%d.png", letter_index);
    IMG_SavePNG(letter_surface, filename);

    SDL_FreeSurface(letter_surface);
}

void detect_characters(SDL_Surface *image) {
    int letter_index = 0;

    for (int y = 0; y < image->h; y += MIN_CHARACTER_SIZE) {
        for (int x = 0; x < image->w; x += MIN_CHARACTER_SIZE) {
            int black_pixels = 0;

            // Compter les pixels noirs dans ce bloc
            for (int j = 0; j < MIN_CHARACTER_SIZE && y + j < image->h; j++) {
                for (int i = 0; i < MIN_CHARACTER_SIZE && x + i < image->w; i++) {
                    Uint32 pixel = ((Uint32 *)image->pixels)[(y + j) * image->w + (x + i)];
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel, image->format, &r, &g, &b);

                    if (r == 0 && g == 0 && b == 0) { // Si le pixel est noir
                        black_pixels++;
                    }
                }
            }

            // Si le bloc contient suffisamment de pixels noirs, on le considère comme une lettre
            if (black_pixels > (MIN_CHARACTER_SIZE * MIN_CHARACTER_SIZE) / 4) {
                save_letter(image, x, y, MIN_CHARACTER_SIZE, MIN_CHARACTER_SIZE, letter_index++);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *grid_image = IMG_Load("grid_only.png");
    if (!grid_image) {
        fprintf(stderr, "Erreur lors du chargement de l'image: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Étape 1 : Binarisation de l'image
    binarize(grid_image);

    // Étape 2 : Détection et sauvegarde des caractères
    detect_characters(grid_image);

    SDL_FreeSurface(grid_image);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

