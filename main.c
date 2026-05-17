#include <stdio.h>
#include <stdlib.h>

#define MAX_LIVRES 100

typedef struct {
    int  id;
    char titre[80];
    char auteur[80];
    char categorie[40];
} Livre;

Livre livres[MAX_LIVRES];
int   nb_livres = 0;

void enlever_retour_ligne(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == '\n' || s[i] == '\r') { s[i] = '\0'; return; }
        i++;
    }
}

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* on jette */ }
}

void charger_livres() {
    FILE *f = fopen("books.txt", "r");
    if (f == NULL) { printf("Erreur : books.txt introuvable.\n"); return; }

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

void afficher_livres() {
    printf("=== Liste des livres (%d) ===\n", nb_livres);
    for (int i = 0; i < nb_livres; i++) {
        printf("  [#%d] %s - %s (%s)\n",
               livres[i].id, livres[i].titre,
               livres[i].auteur, livres[i].categorie);
    }
}

int main() {
    charger_livres();

    int choix;
    while (1) {
        printf("\n=========== CY-biblioTECH ===========\n");
        printf("1. Voir tous les livres\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        if (scanf("%d", &choix) != 1) { vider_buffer(); printf("Invalide.\n"); continue; }
        vider_buffer();

        if (choix == 0) break;
        else if (choix == 1) afficher_livres();
        else printf("Choix invalide.\n");
    }
    return 0;
}
