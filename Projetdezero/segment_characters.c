#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHAR_WIDTH 50
#define MAX_CHAR_HEIGHT 50
#define MIN_CHAR_WIDTH 5
#define MIN_CHAR_HEIGHT 5

int processed_pixels[1000][1000];

int is_black_pixel(SDL_Surface *surface, int x, int y) {
    if (processed_pixels[y][x]) return 0;

    Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);

    return (r == 0 && g == 0 && b == 0);
}

void find_character_bounds(SDL_Surface *surface, int startX, int startY, int *left, int *right, int *top, int *bottom) {
    *left = *right = startX;
    *top = *bottom = startY;

    for (int y = startY; y < surface->h && y <= *bottom + MAX_CHAR_HEIGHT; y++) {
        for (int x = startX; x < surface->w && x <= *right + MAX_CHAR_WIDTH; x++) {
            if (is_black_pixel(surface, x, y)) {
                if (x < *left) *left = x;
                if (x > *right) *right = x;
                if (y < *top) *top = y;
                if (y > *bottom) *bottom = y;
                processed_pixels[y][x] = 1;
            }
        }
    }
}

void save_character(SDL_Surface *surface, int left, int top, int right, int bottom, int index) {
    int charWidth = right - left + 1;
    int charHeight = bottom - top + 1;

    if (charWidth < MIN_CHAR_WIDTH || charHeight < MIN_CHAR_HEIGHT) return;

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

// Vérifie si une ligne est vide
int is_empty_line(SDL_Surface *surface, int y) {
    for (int x = 0; x < surface->w; x++) {
        if (is_black_pixel(surface, x, y)) return 0;
    }
    return 1;
}

void segment_characters(SDL_Surface *surface) {
    int index = 0;
    memset(processed_pixels, 0, sizeof(processed_pixels));

    int y = 0;
    while (y < surface->h) {
        if (is_empty_line(surface, y)) {
            y++;  // Passe à la ligne suivante si elle est vide
            continue;
        }

        for (int x = 0; x < surface->w; x++) {
            if (is_black_pixel(surface, x, y)) {
                int left, right, top, bottom;
                find_character_bounds(surface, x, y, &left, &right, &top, &bottom);

                save_character(surface, left, top, right, bottom, index);
                index++;

                x = right;  // Passe au pixel suivant après le caractère
            }
        }

        // Passe à la prochaine ligne vide après avoir traité une ligne pleine
        while (y < surface->h && !is_empty_line(surface, y)) {
            y++;
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

    segment_characters(imageSurface);

    SDL_FreeSurface(imageSurface);
    SDL_Quit();

    return 0;
}

