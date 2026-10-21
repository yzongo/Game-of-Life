# Compilateur utilisé
CC = gcc

# Flags de COMPILATION (appliqués à chaque .c -> .o)
# -g       : infos de débogage (pour gdb)
# -Wall    : active les avertissements utiles (fortement recommandé)
CFLAGS = -g -Wall

# Flags de LIAISON (appliqués une seule fois, à la toute fin,
# quand on assemble tous les .o en un seul exécutable)
# -lSDL2 : lie la bibliothèque SDL2
LDFLAGS = -lSDL2

# Liste des fichiers objets à produire
OBJ = life.o main.o

# Nom du fichier exécutable final
TARGET = executable

# Règle par défaut (la première du fichier = celle exécutée par "make" seul)
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Règles explicites de compilation (un .o par .c)
life.o: life.c life.h
	$(CC) $(CFLAGS) -c life.c -o life.o

main.o: main.c life.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ) $(TARGET)

# Indique à make que "clean" n'est pas un vrai fichier à produire
.PHONY: clean
