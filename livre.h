#ifndef LIVRE_H
#define LIVRE_H

#define MAX_LIVRES 100

typedef struct {
    int  id;
    char titre[80];
    char auteur[80];
    char categorie[40];
} Livre;

extern Livre livres[MAX_LIVRES];
extern int   nb_livres;

void charger_livres();
void sauver_livres();
int  trouver_livre(int id);
void afficher_livre(int i);
void trier_livres(char mode);
void ajouter_livre();

#endif
