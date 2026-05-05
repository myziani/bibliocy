#include <stdio.h>
#include <string.h>
#define MAX_LIVRES 100

typedef struct {
    char titre[50];
    char auteur[50];
    int annee;
} Livre;


Livre bibliotheque[MAX_LIVRES];
int nbLivres = 0;

void enleverRetourLigne(char chaine[]) {
        chaine[strcspn(chaine, "\n")] = '\0';
}

int chargerLivres(Livre livres[]) {
        FILE *fichier = fopen("livres.txt", "r");

if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return 0;
}

int nb = 0;

while (nb < MAX_LIVRES && fgets(livres[nb].titre, 100, fichier) != NULL) {

    enleverRetourLigne(livres[nb].titre);

    fgets(livres[nb].auteur, 100, fichier);
    enleverRetourLigne(livres[nb].auteur);

    fscanf(fichier, "%d\n", &livres[nb].annee);

    nb++;
}

fclose(fichier);
    return nb;
}

void afficherLivres(Livre livres[], int nbLivres) {
    for (int i = 0; i < nbLivres; i++) {
        printf("Livre %d :\n", i + 1);
        printf("Titre : %s\n", livres[i].titre);
        printf("Auteur : %s\n", livres[i].auteur);
        printf("Annee : %d\n\n", livres[i].annee);
    }
}


int main() {

    Livre livres[MAX_LIVRES];

    int nbLivres = chargerLivres(livres);

    afficherLivres(livres, nbLivres);

    return 0;
}
