# ===========================================================
# CY-biblioTECH - Makefile
# ===========================================================
# Lance simplement :
#   make          -> compile (uniquement ce qui a change)
#   make run      -> compile puis lance le programme
#   make clean    -> supprime les .o et le binaire
# ===========================================================

# --- Variables (comme des #define en C) ------------------------------
CC      = gcc
CFLAGS  = -Wall -std=c99 -O2
TARGET  = bibliotech

# Liste des fichiers sources et la liste des .o correspondants.
# $(SRC:.c=.o) veut dire "prends SRC et remplace .c par .o".
SRC     = main.c livre.c utilisateur.c emprunt.c
OBJ     = $(SRC:.c=.o)

# --- Cibles ----------------------------------------------------------

# Cible par defaut (quand on tape juste "make").
all: $(TARGET)

# Lien : on assemble tous les .o en un binaire executable.
#   $@  = le nom de la cible (ici : bibliotech)
#   $^  = la liste de toutes les dependances (ici : les .o)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regle generique : comment fabriquer un fichier .o depuis un .c.
#   $<  = la premiere dependance (le .c source)
#   $@  = la cible (le .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependances aux fichiers .h : si livre.h change, on doit
# recompiler tous les .o qui incluent livre.h.
main.o:        main.c livre.h utilisateur.h emprunt.h
livre.o:       livre.c livre.h
utilisateur.o: utilisateur.c utilisateur.h
emprunt.o:     emprunt.c emprunt.h livre.h utilisateur.h

# --- Cibles utilitaires (ce ne sont pas des fichiers) ----------------

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean
