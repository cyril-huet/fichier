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

// Fonction pour lire une image BMP en niveaux de gris
Image load_bmp_image(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", filename);
        exit(1);
    }

    // Lire l'en-tête BMP
    fseek(fp, 18, SEEK_SET);
    int width, height;
    fread(&width, sizeof(int), 1, fp);
    fread(&height, sizeof(int), 1, fp);

    // Assurez-vous que l'image est en niveaux de gris
    fseek(fp, 28, SEEK_SET);
    uint16_t bit_count;
    fread(&bit_count, sizeof(uint16_t), 1, fp);
    if (bit_count != 8) {
        fprintf(stderr, "L'image BMP doit être en niveaux de gris avec un bit depth de 8.\n");
        fclose(fp);
        exit(1);
    }

    // Créer une image
    Image img = create_image(width, height);
    fseek(fp, 54, SEEK_SET);  // Passer l'en-tête BMP pour atteindre les données des pixels

    // Lire les données des pixels
    for (int y = height - 1; y >= 0; y--) {  // BMP stocke les lignes de bas en haut
        fread(img.data + y * width, sizeof(uint8_t), width, fp);
    }

    fclose(fp);
    return img;
}

// Fonction pour sauvegarder une image BMP
void save_bmp_image(const char *filename, Image img) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", filename);
        exit(1);
    }

    // Écrire l'en-tête BMP
    uint16_t bmp_signature = 0x4D42; // "BM"
    fwrite(&bmp_signature, sizeof(uint16_t), 1, fp);
    uint32_t file_size = 54 + img.width * img.height; // Taille totale du fichier
    fwrite(&file_size, sizeof(uint32_t), 1, fp);
    uint32_t reserved = 0;
    fwrite(&reserved, sizeof(uint32_t), 1, fp);
    uint32_t data_offset = 54; // Offset vers les données de l'image
    fwrite(&data_offset, sizeof(uint32_t), 1, fp);
    
    // Écrire l'en-tête DIB
    uint32_t dib_header_size = 40;
    fwrite(&dib_header_size, sizeof(uint32_t), 1, fp);
    fwrite(&img.width, sizeof(int), 1, fp);
    fwrite(&img.height, sizeof(int), 1, fp);
    uint16_t planes = 1;
    fwrite(&planes, sizeof(uint16_t), 1, fp);
    uint16_t bit_count = 8; // Niveaux de gris
    fwrite(&bit_count, sizeof(uint16_t), 1, fp);
    uint32_t compression = 0;
    fwrite(&compression, sizeof(uint32_t), 1, fp);
    uint32_t image_size = img.width * img.height; // Taille des données de l'image
    fwrite(&image_size, sizeof(uint32_t), 1, fp);
    uint32_t x_pixels_per_meter = 2835;
    fwrite(&x_pixels_per_meter, sizeof(uint32_t), 1, fp);
    uint32_t y_pixels_per_meter = 2835;
    fwrite(&y_pixels_per_meter, sizeof(uint32_t), 1, fp);
    uint32_t total_colors = 256;
    fwrite(&total_colors, sizeof(uint32_t), 1, fp);
    uint32_t important_colors = 256;
    fwrite(&important_colors, sizeof(uint32_t), 1, fp);

    // Écrire la palette de couleurs (niveaux de gris)
    for (int i = 0; i < 256; i++) {
        fwrite(&i, sizeof(uint8_t), 1, fp);
        fwrite(&i, sizeof(uint8_t), 1, fp);
        fwrite(&i, sizeof(uint8_t), 1, fp);
        fwrite(&reserved, sizeof(uint8_t), 1, fp); // Réservé
    }

    // Écrire les données des pixels
    for (int y = img.height - 1; y >= 0; y--) {
        fwrite(img.data + y * img.width, sizeof(uint8_t), img.width, fp);
    }

    fclose(fp);
}

// Fonction pour binariser l'image
void binarize_image(Image *img) {
    for (int i = 0; i < img->width * img->height; i++) {
        img->data[i] = (img->data[i] < THRESHOLD) ? 0 : 255;
    }
}

// Fonction pour détecter les contours des lettres et extraire les lettres
void detect_and_extract_letters(Image *src) {
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

                // DFS pour trouver les contours de la lettre
                int stack[src->height * src->width][2];
                int stack_size = 0;
                stack[stack_size][0] = x;
                stack[stack_size][1] = y;
                stack_size++;

                while (stack_size > 0) {
                    stack_size--;
                    int cur_x = stack[stack_size][0];
                    int cur_y = stack[stack_size][1];

                    if (cur_x < 0 || cur_x >= src->width || cur_y < 0 || cur_y >= src->height || visited[cur_y][cur_x]) {
                        continue;
                    }

                    visited[cur_y][cur_x] = 1;

                    if (src->data[cur_y * src->width + cur_x] == 0) {
                        if (cur_x < min_x) min_x = cur_x;
                        if (cur_y < min_y) min_y = cur_y;
                        if (cur_x > max_x) max_x = cur_x;
                        if (cur_y > max_y) max_y = cur_y;

                        stack[stack_size][0] = cur_x + 1;
                        stack[stack_size][1] = cur_y;
                        stack_size++;
                        stack[stack_size][0] = cur_x - 1;
                        stack[stack_size][1] = cur_y;
                        stack_size++;
                        stack[stack_size][0] = cur_x;
                        stack[stack_size][1] = cur_y + 1;
                        stack_size++;
                        stack[stack_size][0] = cur_x;
                        stack[stack_size][1] = cur_y - 1;
                        stack_size++;
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

                    char filename[50];
                    snprintf(filename, sizeof(filename), "letter_%d.bmp", label);
                    save_bmp_image(filename, letter);
                    free_image(letter);
                    label++;
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_image.bmp>\n", argv[0]);
        return 1;
    }

    // Charger l'image source
    Image src = load_bmp_image(argv[1]);
    
    // Binariser l'image
    binarize_image(&src);
    
    // Détecter les lettres et les extraire
    detect_and_extract_letters(&src);
    
    // Libérer la mémoire de l'image source
    free_image(src);
    
    return 0;
}

