/* utilisateur.h
   Module qui gere les comptes (etudiants 'E' et professeurs 'P'). */
#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#define MAX_USERS 50

/* Mot de passe "maitre" demande quand quelqu'un veut creer un compte
   professeur. Empeche les etudiants de s'auto-promouvoir en prof. */
#define MDP_PROF_MASTER "biblio2026"

typedef struct {
    char login[30];
    char mdp[30];
    char role;       /* 'E' = etudiant, 'P' = professeur */
} Utilisateur;

extern Utilisateur users[MAX_USERS];
extern int         nb_users;

void charger_users();
void sauver_users();
int  trouver_user(char login[]);     /* renvoie l'indice ou -1 */
void inscription();
int  connexion();                    /* renvoie l'indice du user, ou -1 */

#endif
