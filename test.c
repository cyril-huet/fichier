#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>

void ResizeImage(const char *input_file, const char *output_file, const char *size) {
    MagickWandGenesis(); // Initialisation de la bibliothèque MagickWand

    MagickWand *wand = NewMagickWand();
    
    // Chargement de l'image
    if (MagickReadImage(wand, input_file) == MagickFalse) {
        fprintf(stderr, "Erreur lors de la lecture de l'image %s\n", input_file);
        DestroyMagickWand(wand);
        MagickWandTerminus();
        return;
    }

    // Redimensionnement de l'image
    if (MagickResizeImage(wand, atoi(size), atoi(size), LanczosFilter, 1.0) == MagickFalse) {
        fprintf(stderr, "Erreur lors du redimensionnement de l'image\n");
        DestroyMagickWand(wand);
        MagickWandTerminus();
        return;
    }

    // Sauvegarde de l'image redimensionnée
    if (MagickWriteImage(wand, output_file) == MagickFalse) {
        fprintf(stderr, "Erreur lors de l'écriture de l'image %s\n", output_file);
    }

    // Libération des ressources
    DestroyMagickWand(wand);
    MagickWandTerminus(); // Terminaison de la bibliothèque MagickWand
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Utilisation: %s <input_file> <output_file> <size>\n", argv[0]);
        return 1;
    }

    ResizeImage(argv[1], argv[2], argv[3]);

    return 0;
}

