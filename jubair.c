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

// Fonction pour binariser l'image
void binarize_image(Image *img) {
    for (int i = 0; i < img->width * img->height; i++) {
        img->data[i] = (img->data[i] < THRESHOLD) ? 0 : 255;
    }
}

// Fonction pour sauvegarder une sous-image dans un tableau de fichiers
void save_subimage(const char *base_filename, Image *letter, int label) {
    // Créer le nom de fichier pour la sous-image
    char filename[50];
    snprintf(filename, sizeof(filename), "%s_letter_%d.pgm", base_filename, label);
    
    // Sauvegarder en format PGM (Portable Gray Map)
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", filename);
        exit(1);
    }
    
    fprintf(fp, "P5\n%d %d\n255\n", letter->width, letter->height);
    fwrite(letter->data, sizeof(uint8_t), letter->width * letter->height, fp);
    
    fclose(fp);
}

// Fonction pour détecter les contours des lettres et extraire les lettres
void detect_and_extract_letters(Image *src, const char *base_filename) {
    int visited[src->height][src->width];
    for (int i = 0; i < src->height; i++) {
        for (int j = 0; j < src->width; j++) {
            visited[i][j] = 0;
        }
    }
    
    int label = 0;

    // Détection des contours des lettres
    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            if (src->data[y * src->width + x] == 0 && !visited[y][x]) {
                // Début d'une nouvelle lettre
                int min_x = x, min_y = y, max_x = x, max_y = y;

                // BFS pour trouver les contours de la lettre
                int queue[src->height * src->width][2];
                int queue_size = 0;
                queue[queue_size][0] = x;
                queue[queue_size][1] = y;
                queue_size++;

                while (queue_size > 0) {
                    int cur_x = queue[0][0];
                    int cur_y = queue[0][1];
                    queue_size--;

                    // Marquer le pixel comme visité
                    visited[cur_y][cur_x] = 1;

                    // Mettre à jour les coordonnées de la lettre
                    if (cur_x < min_x) min_x = cur_x;
                    if (cur_y < min_y) min_y = cur_y;
                    if (cur_x > max_x) max_x = cur_x;
                    if (cur_y > max_y) max_y = cur_y;

                    // Ajouter les voisins non visités à la file d'attente
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0)) continue; // Éviter de prendre les diagonales
                            int new_x = cur_x + dx;
                            int new_y = cur_y + dy;
                            if (new_x >= 0 && new_x < src->width && new_y >= 0 && new_y < src->height &&
                                !visited[new_y][new_x] && src->data[new_y * src->width + new_x] == 0) {
                                queue[queue_size][0] = new_x;
                                queue[queue_size][1] = new_y;
                                queue_size++;
                            }
                        }
                    }
                }

                // Extraire et sauvegarder la lettre
                int letter_width = max_x - min_x + 1;
                int letter_height = max_y - min_y + 1;
                
                if (letter_width > 0 && letter_height > 0) {
                    Image letter = create_image(letter_width, letter_height);
                    for (int ly = 0; ly < letter_height; ly++) {
                        for (int lx = 0; lx < letter_width; lx++) {
                            letter.data[ly * letter_width + lx] = src->data[(min_y + ly) * src->width + (min_x + lx)];
                        }
                    }

                    // Sauvegarder la lettre
                    save_subimage(base_filename, &letter, label);
                    free_image(letter);
                    label++;
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_image.pgm>\n", argv[0]);
        return 1;
    }

    // Charger l'image source depuis un fichier PGM
    // Vous devez remplacer cette partie par une méthode appropriée pour charger l'image
    // Par exemple, utiliser `fopen` et `fread` pour lire l'image PGM

    // Pour l'exemple, nous allons initialiser une image vide ici
    Image src = create_image(100, 100); // Changez la taille selon vos besoins
    // Remplir l'image src.data avec les données de l'image chargée ici

    // Binariser l'image
    binarize_image(&src);
    
    // Détecter les lettres et les extraire
    detect_and_extract_letters(&src, "output");
    
    // Libérer la mémoire de l'image source
    free_image(src);
    
    return 0;
}

