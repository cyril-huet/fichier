#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>

int convert_to_bmp(const char *input_file) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return -1;
    }

    // Initialisation de SDL_image
    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))) {
        fprintf(stderr, "Erreur IMG_Init: %s\n", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    // Charger l'image avec SDL_image
    SDL_Surface *image = IMG_Load(input_file);
    if (!image) {
        fprintf(stderr, "Erreur de chargement d'image: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Sauvegarder l'image en BMP avec le nom "wordsearch.bmp"
    const char *output_file = "wordsearch.bmp";
    if (SDL_SaveBMP(image, output_file) != 0) {
        fprintf(stderr, "Erreur de sauvegarde BMP: %s\n", SDL_GetError());
        SDL_FreeSurface(image);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    printf("Image convertie en BMP et sauvegardée sous le nom %s\n", output_file);

    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_source>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];

    return convert_to_bmp(input_file);
}

