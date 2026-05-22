/* livre.c - Implementation du module Livre. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livre.h"

/* Fonctions utiles definies dans main.c */
void enlever_retour_ligne(char chaine[]);
int  lire_ligne_utile(FILE *f, char buf[], int taille);

/* Definition des variables globales declarees dans livre.h */
Livre livres[MAX_LIVRES];
int   nb_livres = 0;

void charger_livres() {
    FILE *f = fopen("books.txt", "r");
    if (f == NULL) return;   /* pas de fichier = bibliotheque vide */

    nb_livres = 0;
    char ligne[100];
    while (nb_livres < MAX_LIVRES) {
        if (!lire_ligne_utile(f, ligne, 100)) break;
        livres[nb_livres].id = atoi(ligne);
        if (!lire_ligne_utile(f, livres[nb_livres].titre,     80)) break;
        if (!lire_ligne_utile(f, livres[nb_livres].auteur,    80)) break;
        if (!lire_ligne_utile(f, livres[nb_livres].categorie, 40)) break;
        nb_livres++;
    }
    fclose(f);
}

void sauver_livres() {
    FILE *f = fopen("books.txt", "w");
    if (f == NULL) return;
    fprintf(f, "# CY-biblioTECH - Liste des livres\n");
    fprintf(f, "# 4 lignes par livre : id, titre, auteur, categorie\n");
    fprintf(f, "# Les lignes vides et les lignes commencant par # sont ignorees.\n");
    fprintf(f, "\n");
    for (int i = 0; i < nb_livres; i++) {
        fprintf(f, "# --- Livre #%d ---\n", livres[i].id);
        fprintf(f, "%d\n", livres[i].id);
        fprintf(f, "%s\n", livres[i].titre);
        fprintf(f, "%s\n", livres[i].auteur);
        fprintf(f, "%s\n", livres[i].categorie);
        fprintf(f, "\n");
    }
    fclose(f);
}

int trouver_livre(int id) {
    for (int i = 0; i < nb_livres; i++) {
        if (livres[i].id == id) return i;
    }
    return -1;
}

void afficher_livre(int i) {
    printf("  [#%d] %-25s | %-20s | %s\n",
           livres[i].id, livres[i].titre, livres[i].auteur, livres[i].categorie);
}

void trier_livres(char mode) {
    /* Tri a bulles tout simple. */
    for (int i = 0; i < nb_livres - 1; i++) {
        for (int j = 0; j < nb_livres - 1 - i; j++) {
            char *a, *b;
            if (mode == 'a')      { a = livres[j].auteur;    b = livres[j+1].auteur;    }
            else if (mode == 'c') { a = livres[j].categorie; b = livres[j+1].categorie; }
            else                  { a = livres[j].titre;     b = livres[j+1].titre;     }

            if (strcmp(a, b) > 0) {
                Livre tmp   = livres[j];
                livres[j]   = livres[j+1];
                livres[j+1] = tmp;
            }
        }
    }
}

void ajouter_livre() {
    if (nb_livres >= MAX_LIVRES) {
        printf("Bibliotheque pleine.\n");
        return;
    }

    /* Nouvel id = plus grand id existant + 1. */
    int nouveau_id = 1;
    for (int i = 0; i < nb_livres; i++) {
        if (livres[i].id >= nouveau_id) nouveau_id = livres[i].id + 1;
    }

    printf("Titre     : ");
    fgets(livres[nb_livres].titre, 80, stdin);
    enlever_retour_ligne(livres[nb_livres].titre);

    printf("Auteur    : ");
    fgets(livres[nb_livres].auteur, 80, stdin);
    enlever_retour_ligne(livres[nb_livres].auteur);

    printf("Categorie : ");
    fgets(livres[nb_livres].categorie, 40, stdin);
    enlever_retour_ligne(livres[nb_livres].categorie);

    livres[nb_livres].id = nouveau_id;
    nb_livres++;
    sauver_livres();
    printf("Livre ajoute avec l'id %d.\n", nouveau_id);
}
