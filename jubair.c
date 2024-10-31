#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define THRESHOLD 128  // Seuil pour binarisation

// Structure pour une image en niveaux de gris
typedef struct {
    uint8_t *data;
    int width;
    int height;
} Image;

// Fonction pour créer une nouvelle image
Image create_image(int width, int height) {
    Image img;
    img.width = width;
    img.height = height;
    img.data = (uint8_t *)malloc(width * height);
    if (img.data == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour l'image.\n");
        exit(1);
    }
    return img;
}

// Fonction pour libérer la mémoire d'une image
void free_image(Image img) {
    free(img.data);
}

// Fonction pour lire une image PGM en niveaux de gris
Image load_pgm_image(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", filename);
        exit(1);
    }

    char format[3];
    int width, height, max_val;

    // Lecture de l'en-tête PGM
    fscanf(fp, "%2s\n%d %d\n%d\n", format, &width, &height, &max_val);
    if (format[0] != 'P' || format[1] != '5' || max_val != 255) {
        fprintf(stderr, "Le fichier PGM doit être en format P5 avec max_val = 255.\n");
        fclose(fp);
        exit(1);
    }

    Image img = create_image(width, height);
    fread(img.data, 1, width * height, fp);

    fclose(fp);
    return img;
}

// Fonction pour sauvegarder une image au format PGM
void save_pgm_image(const char *filename, Image img) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", filename);

