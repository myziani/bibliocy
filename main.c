#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIVRES 100
#define MAX_USERS  50

typedef struct {
    int  id;
    char titre[80];
    char auteur[80];
    char categorie[40];
} Livre;

typedef struct {
    char login[30];
    char mdp[30];
    char role;        /* 'E' = etudiant, 'P' = professeur (pas encore utilise) */
} Utilisateur;

Livre       livres[MAX_LIVRES];
int         nb_livres = 0;
Utilisateur users[MAX_USERS];
int         nb_users = 0;

void enlever_retour_ligne(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == '\n' || s[i] == '\r') { s[i] = '\0'; return; }
        i++;
    }
}

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void charger_livres() {
    FILE *f = fopen("books.txt", "r");
    if (f == NULL) return;
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

void afficher_livres() {
    printf("=== Liste des livres (%d) ===\n", nb_livres);
    for (int i = 0; i < nb_livres; i++) {
        printf("  [#%d] %s - %s (%s)\n",
               livres[i].id, livres[i].titre,
               livres[i].auteur, livres[i].categorie);
    }
}

int connexion() {
    char login[30], mdp[30];
    printf("Login : ");        scanf("%29s", login); vider_buffer();
    printf("Mot de passe : "); scanf("%29s", mdp);   vider_buffer();
    for (int i = 0; i < nb_users; i++) {
        if (strcmp(users[i].login, login) == 0 &&
            strcmp(users[i].mdp,   mdp)   == 0) {
            printf("Bienvenue %s.\n", login);
            return i;
        }
    }
    printf("Login ou mot de passe incorrect.\n");
    return -1;
}

void menu_connecte(int idx) {
    int choix;
    while (1) {
        printf("\n=== Bonjour %s ===\n", users[idx].login);
        printf("1. Voir tous les livres\n");
        printf("0. Deconnexion\n");
        printf("Choix : ");
        if (scanf("%d", &choix) != 1) { vider_buffer(); continue; }
        vider_buffer();
        if (choix == 0) return;
        else if (choix == 1) afficher_livres();
        else printf("Choix invalide.\n");
    }
}

int main() {
    charger_livres();
    charger_users();
    printf("Charge : %d livres, %d utilisateurs.\n", nb_livres, nb_users);

    int choix;
    while (1) {
        printf("\n=========== CY-biblioTECH ===========\n");
        printf("1. Se connecter\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        if (scanf("%d", &choix) != 1) { vider_buffer(); continue; }
        vider_buffer();

        if (choix == 0) break;
        else if (choix == 1) {
            int idx = connexion();
            if (idx >= 0) menu_connecte(idx);
        }
        else printf("Choix invalide.\n");
    }
    return 0;
}
