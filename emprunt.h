#ifndef EMPRUNT_H
#define EMPRUNT_H

#define MAX_EMPRUNTS 100
#define MAX_ETUDIANT 3
#define MAX_PROF     5
#define DUREE_ETU    120
#define DUREE_PROF   180

typedef struct {
    int  id_livre;
    char login[30];
    long heure_emprunt;
} Emprunt;

extern Emprunt emprunts[MAX_EMPRUNTS];
extern int     nb_emprunts;

void charger_emprunts();
void sauver_emprunts();
int  livre_emprunte(int id);
int  compter_emprunts_user(char login[]);
int  duree_pour(char role);
int  a_des_retards(char login[], char role);
void emprunter(int idx_user);
void rendre(int idx_user);
void afficher_mes_emprunts(char login[], char role);

#endif
