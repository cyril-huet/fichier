#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define TOLERANCE 10 // tolérance pour détecter les lignes de la grille

int *detect_lines(SDL_Surface *image, int *num_lines, int horizontal) {
    int max_dim = horizontal ? image->h : image->w;
    int *lines = (int *)malloc(max_dim * sizeof(int));
    *num_lines = 0;

    for (int i = 0; i < max_dim; i++) {
        int is_line = 1;
        for (int j = 0; j < (horizontal ? image->w : image->h); j++) {
            Uint32 pixel = *((Uint32 *)image->pixels + (horizontal ? i * image->w + j : j * image->w + i));
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            if (r > 200 && g > 200 && b > 200) { // Considérons les pixels clairs comme des espaces
                is_line = 0;
                break;
            }
        }
        if (is_line && (*num_lines == 0 || i - lines[*num_lines - 1] > TOLERANCE)) {
            lines[(*num_lines)++] = i;
        }
    }
    return lines;
}

void save_letter(SDL_Surface *grid, int x, int y, int width, int height, int letter_index) {
    SDL_Rect cell_rect = {x, y, width, height};
    SDL_Surface *letter_surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_BlitSurface(grid, &cell_rect, letter_surface, NULL);

    char filename[50];
    snprintf(filename, sizeof(filename), "letters/letter_%d.png", letter_index);
    IMG_SavePNG(letter_surface, filename);

    SDL_FreeSurface(letter_surface);
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

    int num_horizontal_lines, num_vertical_lines;
    int *horizontal_lines = detect_lines(grid_image, &num_horizontal_lines, 1);
    int *vertical_lines = detect_lines(grid_image, &num_vertical_lines, 0);

    if (num_horizontal_lines < 2 || num_vertical_lines < 2) {
        fprintf(stderr, "Erreur : Impossible de détecter la grille.\n");
        free(horizontal_lines);
        free(vertical_lines);
        SDL_FreeSurface(grid_image);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Découpage et sauvegarde des lettres
    int letter_index = 0;
    for (int row = 0; row < num_horizontal_lines - 1; row++) {
        for (int col = 0; col < num_vertical_lines - 1; col++) {
            int x = vertical_lines[col];
            int y = horizontal_lines[row];
            int width = vertical_lines[col + 1] - x;
            int height = horizontal_lines[row + 1] - y;

            save_letter(grid_image, x, y, width, height, letter_index++);
        }
    }

    // Libération de la mémoire
    free(horizontal_lines);
    free(vertical_lines);
    SDL_FreeSurface(grid_image);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

