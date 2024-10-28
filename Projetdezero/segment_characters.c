#include <SDL2/SDL.h>
#include <stdio.h>

// Dimensions maximales d'un caractère (à ajuster si nécessaire)
#define MAX_CHAR_WIDTH 50
#define MAX_CHAR_HEIGHT 50

// Fonction pour vérifier si un pixel est noir
int is_black_pixel(SDL_Surface *surface, int x, int y) {
    Uint32 pixel = ((Uint32*)surface->pixels)[y * surface->w + x];
    Uint8 r, g, b;
    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
    return (r == 0 && g == 0 && b == 0);
}

// Fonction pour trouver les limites d'un caractère (haut, bas, gauche, droite)
void find_character_bounds(SDL_Surface *surface, int startX, int startY, int *left, int *right, int *top, int *bottom) {
    *left = *right = startX;
    *top = *bottom = startY;

    // Parcours de la zone pour trouver les limites
    for (int y = startY; y < surface->h && y <= *bottom + MAX_CHAR_HEIGHT; y++) {
        for (int x = startX; x < surface->w && x <= *right + MAX_CHAR_WIDTH; x++) {
            if (is_black_pixel(surface, x, y)) {
                if (x < *left) *left = x;
                if (x > *right) *right = x;
                if (y < *top) *top = y;
                if (y > *bottom) *bottom = y;
            }
        }
    }
}

// Fonction pour extraire et enregistrer chaque caractère
void save_character(SDL_Surface *surface, int left, int top, int right, int bottom, int index) {
    int charWidth = right - left + 1;
    int charHeight = bottom - top + 1;

    // Créer une surface pour le caractère
    SDL_Surface *charSurface = SDL_CreateRGBSurface(0, charWidth, charHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!charSurface) {
        fprintf(stderr, "Erreur de création de surface pour caractère: %s\n", SDL_GetError());
        return;
    }

    // Copier les pixels du caractère dans la nouvelle surface
    SDL_Rect srcRect = { left, top, charWidth, charHeight };
    SDL_Rect destRect = { 0, 0, charWidth, charHeight };
    SDL_BlitSurface(surface, &srcRect, charSurface, &destRect);

    // Sauvegarder l'image du caractère
    char filename[100];
    sprintf(filename, "character_%d.bmp", index);
    SDL_SaveBMP(charSurface, filename);
    SDL_FreeSurface(charSurface);
}

void segment_characters(SDL_Surface *surface) {
    int index = 0;  // Compteur pour nommer les fichiers de caractères

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            if (is_black_pixel(surface, x, y)) {
                // Trouver les limites du caractère
                int left, right, top, bottom;
                find_character_bounds(surface, x, y, &left, &right, &top, &bottom);

                // Extraire et sauvegarder le caractère
                save_character(surface, left, top, right, bottom, index);
                index++;

                // Sauter au pixel suivant après le caractère pour éviter les duplications
                x = right;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Charger l'image binarisée
    SDL_Surface *imageSurface = SDL_LoadBMP("binarized_wordsearch.bmp");  // Chemin vers ton image binarisée
    if (!imageSurface) {
        fprintf(stderr, "Erreur lors du chargement de l'image: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Segmenter les caractères et les sauvegarder
    segment_characters(imageSurface);

    // Libérer les ressources
    SDL_FreeSurface(imageSurface);
    SDL_Quit();

    return 0;
}

