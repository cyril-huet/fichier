#include <SDL2/SDL.h>
#include <stdio.h>

// Seuil de binarisation
#define BIN_THRESHOLD 170

void binarize_image(SDL_Surface *surface) {
    if (surface->format->BytesPerPixel != 4) {
        fprintf(stderr, "Format d'image non supporté.\n");
        return;
    }

    Uint32 *pixels = (Uint32 *)surface->pixels;
    int width = surface->w;
    int height = surface->h;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Uint32 pixel = pixels[y * width + x];
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            Uint8 gray = 0.3 * r + 0.59 * g + 0.11 * b;

            if (gray > BIN_THRESHOLD) {
                pixels[y * width + x] = SDL_MapRGB(surface->format, 255, 255, 255);
            } else {
                pixels[y * width + x] = SDL_MapRGB(surface->format, 0, 0, 0);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Word Search Binarization",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur lors de la création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface *imageSurface = SDL_LoadBMP("wordsearch.bmp");  // Remplace par le chemin de ton image
    if (!imageSurface) {
        fprintf(stderr, "Erreur lors du chargement de l'image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Appliquer la binarisation
    binarize_image(imageSurface);

    // Enregistrer l'image binarisée
    if (SDL_SaveBMP(imageSurface, "binarized_wordsearch.bmp") != 0) {
        fprintf(stderr, "Erreur lors de la sauvegarde de l'image binarisée: %s\n", SDL_GetError());
    } else {
        printf("Image binarisée sauvegardée sous 'binarized_wordsearch.bmp'\n");
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!texture) {
        fprintf(stderr, "Erreur lors de la création de la texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

