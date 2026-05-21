/* utilisateur.h - Module qui gere les comptes (etudiants / professeurs). */
#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#define MAX_USERS 50

typedef struct {
    char login[30];
    char mdp[30];
    char role;       /* 'E' = etudiant, 'P' = professeur */
} Utilisateur;

extern Utilisateur users[MAX_USERS];
extern int         nb_users;

void charger_users();
void sauver_users();
int  trouver_user(char login[]);
void inscription();
int  connexion();

#endif
