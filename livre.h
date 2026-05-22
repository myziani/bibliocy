/* livre.h
   Module qui gere les livres de la bibliotheque. */
#ifndef LIVRE_H
#define LIVRE_H

#define MAX_LIVRES 100

typedef struct {
    int  id;
    char titre[80];
    char auteur[80];
    char categorie[40];
} Livre;

/* Tableau global des livres et son compteur.
   Les variables sont definies dans livre.c (le 'extern' ici dit juste
   au compilateur "elles existent quelque part"). */
extern Livre livres[MAX_LIVRES];
extern int   nb_livres;

void charger_livres();
void sauver_livres();
int  trouver_livre(int id);                /* renvoie l'indice ou -1 */
void afficher_livre(int i);
void trier_livres(char mode);              /* 't' titre, 'a' auteur, 'c' categorie */
void ajouter_livre();

#endif
