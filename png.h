#ifndef PNG_H
#define PNG_H

#include <stdint.h>

// Définition du seuil pour la binarisation
#define THRESHOLD 128

// Structure pour une image en niveaux de gris
typedef struct {
    uint8_t *data;
    int width;
    int height;
} Image;

// Fonction pour créer une nouvelle image avec des dimensions données
Image create_image(int width, int height);

// Fonction pour libérer la mémoire d'une image
void free_image(Image img);

// Fonction pour lire une image PNG en niveaux de gris
Image load_png_image(const char *filename);

// Fonction pour sauvegarder une image PNG en niveaux de gris
void save_png_image(const char *filename, Image img);

// Fonction pour binariser l'image selon le seuil THRESHOLD
void binarize_image(Image *img);

// Fonction pour détecter les contours des lettres et extraire chaque lettre
void detect_and_extract_letters(Image *src);

#endif // PNG_H

