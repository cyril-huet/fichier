# Nom de l'exécutable pour chaque programme
CONVERT_EXEC = convert_bmp
AFFICHER_EXEC = afficher_image
BINARIZE_EXEC = binarize
SEGMENT_EXEC = segment_characters

# Fichiers source pour chaque programme
CONVERT_SRC = convert_bmp.c
AFFICHER_SRC = afficher_image.c
BINARIZE_SRC = binarize_image.c
SEGMENT_SRC = segment_characters.c

# Commande de compilation avec SDL2
CC = gcc
CFLAGS = -lSDL2

# Cibles par défaut
all: $(CONVERT_EXEC) $(AFFICHER_EXEC) $(BINARIZE_EXEC) $(SEGMENT_EXEC)

# Règle pour convertir l'image en BMP
$(CONVERT_EXEC): $(CONVERT_SRC)
	$(CC) -o $(CONVERT_EXEC) $(CONVERT_SRC) $(CFLAGS)
	@echo "Compilation de $(CONVERT_EXEC) terminée."

# Règle pour compiler afficher_image
$(AFFICHER_EXEC): $(AFFICHER_SRC)
	$(CC) -o $(AFFICHER_EXEC) $(AFFICHER_SRC) $(CFLAGS)
	@echo "Compilation de $(AFFICHER_EXEC) terminée."

# Règle pour compiler binarize
$(BINARIZE_EXEC): $(BINARIZE_SRC)
	$(CC) -o $(BINARIZE_EXEC) $(BINARIZE_SRC) $(CFLAGS)
	@echo "Compilation de $(BINARIZE_EXEC) terminée."

# Règle pour compiler segment_characters
$(SEGMENT_EXEC): $(SEGMENT_SRC)
	$(CC) -o $(SEGMENT_EXEC) $(SEGMENT_SRC) $(CFLAGS)
	@echo "Compilation de $(SEGMENT_EXEC) terminée."

# Nettoyage des fichiers objets et exécutables
clean:
	rm -f $(CONVERT_EXEC) $(AFFICHER_EXEC) $(BINARIZE_EXEC) $(SEGMENT_EXEC)
	@echo "Nettoyage terminé."

