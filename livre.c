
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livre.h"

/* Fonctions utiles definies dans main.c */
void enlever_retour_ligne(char chaine[]);

Livre livres[MAX_LIVRES];
int   nb_livres = 0;

void charger_livres() {
    FILE *f = fopen("books.txt", "r");
    if (f == NULL) return;
    char ligne[100];
    while (nb_livres < MAX_LIVRES) {
        if (fgets(ligne, 100, f) == NULL) break;
        livres[nb_livres].id = atoi(ligne);
        if (fgets(livres[nb_livres].titre,     80, f) == NULL) break;
        if (fgets(livres[nb_livres].auteur,    80, f) == NULL) break;
        if (fgets(livres[nb_livres].categorie, 40, f) == NULL) break;
        enlever_retour_ligne(livres[nb_livres].titre);
        enlever_retour_ligne(livres[nb_livres].auteur);
        enlever_retour_ligne(livres[nb_livres].categorie);
        nb_livres++;
    }
    fclose(f);
}

void sauver_livres() {
    FILE *f = fopen("books.txt", "w");
    if (f == NULL) return;
    for (int i = 0; i < nb_livres; i++) {
        fprintf(f, "%d\n%s\n%s\n%s\n",
                livres[i].id, livres[i].titre,
                livres[i].auteur, livres[i].categorie);
    }
    fclose(f);
}

int trouver_livre(int id) {
    for (int i = 0; i < nb_livres; i++)
        if (livres[i].id == id) return i;
    return -1;
}

void afficher_livre(int i) {
    printf("  [#%d] %-25s | %-20s | %s\n",
           livres[i].id, livres[i].titre, livres[i].auteur, livres[i].categorie);
}

void trier_livres(char mode) {
    for (int i = 0; i < nb_livres - 1; i++) {
        for (int j = 0; j < nb_livres - 1 - i; j++) {
            char *a, *b;
            if (mode == 'a')      { a = livres[j].auteur;    b = livres[j+1].auteur;    }
            else if (mode == 'c') { a = livres[j].categorie; b = livres[j+1].categorie; }
            else                  { a = livres[j].titre;     b = livres[j+1].titre;     }
            if (strcmp(a, b) > 0) {
                Livre tmp = livres[j];
                livres[j]   = livres[j+1];
                livres[j+1] = tmp;
            }
        }
    }
}

void ajouter_livre() {
    if (nb_livres >= MAX_LIVRES) { printf("Bibliotheque pleine.\n"); return; }
    int nouveau_id = 1;
    for (int i = 0; i < nb_livres; i++)
        if (livres[i].id >= nouveau_id) nouveau_id = livres[i].id + 1;

    printf("Titre     : "); fgets(livres[nb_livres].titre, 80, stdin);
    enlever_retour_ligne(livres[nb_livres].titre);
    printf("Auteur    : "); fgets(livres[nb_livres].auteur, 80, stdin);
    enlever_retour_ligne(livres[nb_livres].auteur);
    printf("Categorie : "); fgets(livres[nb_livres].categorie, 40, stdin);
    enlever_retour_ligne(livres[nb_livres].categorie);

    livres[nb_livres].id = nouveau_id;
    nb_livres++;
    sauver_livres();
    printf("Livre ajoute (id %d).\n", nouveau_id);
}
