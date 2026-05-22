/* utilisateur.c - Implementation du module Utilisateur. */
#include <stdio.h>
#include <string.h>
#include "utilisateur.h"

/* Fonctions utiles definies dans main.c */
void vider_buffer();
int  lire_ligne_utile(FILE *f, char buf[], int taille);
void lire_mdp(char buf[], int taille);

Utilisateur users[MAX_USERS];
int         nb_users = 0;

void charger_users() {
    FILE *f = fopen("users.txt", "r");
    if (f == NULL) return;

    nb_users = 0;
    char ligne[40];
    while (nb_users < MAX_USERS) {
        if (!lire_ligne_utile(f, users[nb_users].login, 30)) break;
        if (!lire_ligne_utile(f, users[nb_users].mdp,   30)) break;
        if (!lire_ligne_utile(f, ligne, 40)) break;
        users[nb_users].role = ligne[0];
        nb_users++;
    }
    fclose(f);
}

void sauver_users() {
    FILE *f = fopen("users.txt", "w");
    if (f == NULL) return;
    fprintf(f, "# CY-biblioTECH - Liste des utilisateurs\n");
    fprintf(f, "# 3 lignes par utilisateur : login, mot de passe, role\n");
    fprintf(f, "# Role : E = etudiant, P = professeur\n");
    fprintf(f, "\n");
    for (int i = 0; i < nb_users; i++) {
        fprintf(f, "# --- %s ---\n", users[i].login);
        fprintf(f, "%s\n", users[i].login);
        fprintf(f, "%s\n", users[i].mdp);
        fprintf(f, "%c\n", users[i].role);
        fprintf(f, "\n");
    }
    fclose(f);
}

int trouver_user(char login[]) {
    for (int i = 0; i < nb_users; i++) {
        if (strcmp(users[i].login, login) == 0) return i;
    }
    return -1;
}

void inscription() {
    if (nb_users >= MAX_USERS) {
        printf("Trop d'utilisateurs deja enregistres.\n");
        return;
    }

    char login[30], mdp[30], role_buf[5];

    printf("Nouveau login : ");
    scanf("%29s", login);
    vider_buffer();

    if (trouver_user(login) != -1) {
        printf("Ce login existe deja.\n");
        return;
    }

    printf("Mot de passe : ");
    lire_mdp(mdp, 30);

    printf("Role ('E'=etudiant, 'P'=professeur) : ");
    scanf("%4s", role_buf);
    vider_buffer();

    char role = role_buf[0];
    if (role >= 'a' && role <= 'z') role = role - 'a' + 'A';
    if (role != 'E' && role != 'P') {
        printf("Role invalide.\n");
        return;
    }

    /* Pour un compte professeur on demande le mot de passe maitre.
       Ca evite que n'importe quel etudiant se cree un compte prof. */
    if (role == 'P') {
        char mdp_master[30];
        printf("Mot de passe professeur : ");
        lire_mdp(mdp_master, 30);
        if (strcmp(mdp_master, MDP_PROF_MASTER) != 0) {
            printf("Mot de passe professeur incorrect. Compte non cree.\n");
            return;
        }
    }

    strcpy(users[nb_users].login, login);
    strcpy(users[nb_users].mdp,   mdp);
    users[nb_users].role = role;
    nb_users++;
    sauver_users();
    printf("Compte cree.\n");
}

int connexion() {
    char login[30], mdp[30];

    printf("Login : ");
    scanf("%29s", login);
    vider_buffer();

    printf("Mot de passe : ");
    lire_mdp(mdp, 30);

    int idx = trouver_user(login);
    if (idx == -1 || strcmp(users[idx].mdp, mdp) != 0) {
        printf("Login ou mot de passe incorrect.\n");
        return -1;
    }
    printf("Connecte en tant que %s.\n", login);
    return idx;
}
