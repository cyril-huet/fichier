#include <SDL2/SDL.h>
#include <stdio.h>

// Dimensions de la fenêtre (à adapter en fonction de la taille de ton image)
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Créer une fenêtre SDL
    SDL_Window *window = SDL_CreateWindow("Word Search Viewer",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Créer un renderer pour dessiner dans la fenêtre
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur lors de la création du renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Charger l'image
    SDL_Surface *imageSurface = SDL_LoadBMP("wordsearch.bmp"); // Remplace par le chemin de ton image
    if (!imageSurface) {
        fprintf(stderr, "Erreur lors du chargement de l'image: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Créer une texture à partir de l'image
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);  // Libérer la surface après utilisation
    if (!texture) {
        fprintf(stderr, "Erreur lors de la création de la texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Boucle principale pour afficher l'image
    int running = 1;
    SDL_Event event;
    while (running) {
        // Gérer les événements (fermeture de fenêtre, etc.)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Effacer l'écran
        SDL_RenderClear(renderer);
        
        // Copier la texture dans le renderer
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        
        // Afficher l'image à l'écran
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

