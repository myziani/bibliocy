/* utilisateur.c - Implementation du module Utilisateur. */
#include <stdio.h>
#include <string.h>
#include "utilisateur.h"

/* Fonctions utiles definies dans main.c */
void enlever_retour_ligne(char chaine[]);
void vider_buffer();

Utilisateur users[MAX_USERS];
int         nb_users = 0;

void charger_users() {
    FILE *f = fopen("users.txt", "r");
    if (f == NULL) return;
    char ligne[40];
    while (nb_users < MAX_USERS) {
        if (fgets(users[nb_users].login, 30, f) == NULL) break;
        if (fgets(users[nb_users].mdp,   30, f) == NULL) break;
        if (fgets(ligne, 40, f) == NULL) break;
        enlever_retour_ligne(users[nb_users].login);
        enlever_retour_ligne(users[nb_users].mdp);
        users[nb_users].role = ligne[0];
        nb_users++;
    }
    fclose(f);
}

void sauver_users() {
    FILE *f = fopen("users.txt", "w");
    if (f == NULL) return;
    for (int i = 0; i < nb_users; i++)
        fprintf(f, "%s\n%s\n%c\n", users[i].login, users[i].mdp, users[i].role);
    fclose(f);
}

int trouver_user(char login[]) {
    for (int i = 0; i < nb_users; i++)
        if (strcmp(users[i].login, login) == 0) return i;
    return -1;
}

void inscription() {
    if (nb_users >= MAX_USERS) return;
    char login[30], mdp[30], role_buf[5];
    printf("Nouveau login : "); scanf("%29s", login); vider_buffer();
    if (trouver_user(login) != -1) { printf("Login deja pris.\n"); return; }
    printf("Mot de passe : "); scanf("%29s", mdp); vider_buffer();
    printf("Role ('E'/'P') : "); scanf("%4s", role_buf); vider_buffer();
    char role = role_buf[0];
    if (role >= 'a' && role <= 'z') role = role - 'a' + 'A';
    if (role != 'E' && role != 'P') { printf("Role invalide.\n"); return; }
    strcpy(users[nb_users].login, login);
    strcpy(users[nb_users].mdp,   mdp);
    users[nb_users].role = role;
    nb_users++;
    sauver_users();
    printf("Compte cree.\n");
}

int connexion() {
    char login[30], mdp[30];
    printf("Login : "); scanf("%29s", login); vider_buffer();
    printf("Mot de passe : "); scanf("%29s", mdp); vider_buffer();
    int idx = trouver_user(login);
    if (idx == -1 || strcmp(users[idx].mdp, mdp) != 0) {
        printf("Login ou mot de passe incorrect.\n"); return -1;
    }
    printf("Bienvenue %s.\n", login);
    return idx;
}
