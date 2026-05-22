/* emprunt.h
   Module qui gere les emprunts et applique les regles de la bibliotheque. */
#ifndef EMPRUNT_H
#define EMPRUNT_H

#define MAX_EMPRUNTS 100
#define MAX_ETUDIANT 3       /* livres simultanes pour un etudiant */
#define MAX_PROF     5       /* livres simultanes pour un prof    */
#define DUREE_ETU    120     /* 2 minutes en secondes */
#define DUREE_PROF   180     /* 3 minutes en secondes */

typedef struct {
    int  id_livre;
    char login[30];
    long heure_emprunt;  /* timestamp Unix renvoye par time(NULL) */
} Emprunt;

extern Emprunt emprunts[MAX_EMPRUNTS];
extern int     nb_emprunts;

void charger_emprunts();
void sauver_emprunts();

int  livre_emprunte(int id);                       /* 1 si quelqu'un l'a, 0 sinon */
int  compter_emprunts_user(char login[]);
int  duree_pour(char role);                        /* DUREE_ETU ou DUREE_PROF */
int  a_des_retards(char login[], char role);

void emprunter(int idx_user);
void rendre(int idx_user);
void afficher_mes_emprunts(char login[], char role);

#endif
