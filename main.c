#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIVRES   100
#define MAX_USERS    50
#define MAX_EMPRUNTS 100


typedef struct {
    int  id;
    char titre[80];
    char auteur[80];
    char categorie[40];
} Livre;

typedef struct {
    char login[30];
    char mdp[30];
    char role;
} Utilisateur;

typedef struct {
    int  id_livre;
    char login[30];
} Emprunt;

Livre       livres[MAX_LIVRES];     int nb_livres = 0;
Utilisateur users[MAX_USERS];       int nb_users = 0;
Emprunt     emprunts[MAX_EMPRUNTS]; int nb_emprunts = 0;

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
    FILE *f = fopen("books.txt", "r"); if (f == NULL) return;
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
    FILE *f = fopen("users.txt", "r"); if (f == NULL) return;
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
    FILE *f = fopen("users.txt", "w"); if (f == NULL) return;
    for (int i = 0; i < nb_users; i++) {
        fprintf(f, "%s\n%s\n%c\n", users[i].login, users[i].mdp, users[i].role);
    }
    fclose(f);
}

void charger_emprunts() {
    FILE *f = fopen("loans.txt", "r"); if (f == NULL) return;
    char ligne[40];
    while (nb_emprunts < MAX_EMPRUNTS) {
        if (fgets(ligne, 40, f) == NULL) break;
        emprunts[nb_emprunts].id_livre = atoi(ligne);
        if (fgets(emprunts[nb_emprunts].login, 30, f) == NULL) break;
        enlever_retour_ligne(emprunts[nb_emprunts].login);
        nb_emprunts++;
    }
    fclose(f);
}

void sauver_emprunts() {
    FILE *f = fopen("loans.txt", "w"); if (f == NULL) return;
    for (int i = 0; i < nb_emprunts; i++) {
        fprintf(f, "%d\n%s\n", emprunts[i].id_livre, emprunts[i].login);
    }
    fclose(f);
}

int trouver_user(char login[]) {
    for (int i = 0; i < nb_users; i++)
        if (strcmp(users[i].login, login) == 0) return i;
    return -1;
}

int trouver_livre(int id) {
    for (int i = 0; i < nb_livres; i++)
        if (livres[i].id == id) return i;
    return -1;
}

int livre_emprunte(int id) {
    for (int i = 0; i < nb_emprunts; i++)
        if (emprunts[i].id_livre == id) return 1;
    return 0;
}

void afficher_livres_dispo() {
    printf("=== Livres disponibles ===\n");
    for (int i = 0; i < nb_livres; i++) {
        if (!livre_emprunte(livres[i].id)) {
            printf("  [#%d] %s - %s (%s)\n",
                   livres[i].id, livres[i].titre,
                   livres[i].auteur, livres[i].categorie);
        }
    }
}

void afficher_mes_emprunts(char login[]) {
    printf("Vos emprunts :\n");
    int n = 0;
    for (int i = 0; i < nb_emprunts; i++) {
        if (strcmp(emprunts[i].login, login) == 0) {
            int idx = trouver_livre(emprunts[i].id_livre);
            char *titre = (idx >= 0) ? livres[idx].titre : "(inconnu)";
            printf("  [#%d] %s\n", emprunts[i].id_livre, titre);
            n++;
        }
    }
    if (n == 0) printf("  (aucun)\n");
}

void emprunter(char login[]) {
    if (nb_emprunts >= MAX_EMPRUNTS) { printf("Plein.\n"); return; }
    afficher_livres_dispo();
    printf("Id du livre a emprunter : ");
    int id;
    if (scanf("%d", &id) != 1) { vider_buffer(); printf("Invalide.\n"); return; }
    vider_buffer();
    if (trouver_livre(id) == -1) { printf("Inexistant.\n"); return; }
    if (livre_emprunte(id))      { printf("Deja emprunte.\n"); return; }

    emprunts[nb_emprunts].id_livre = id;
    strcpy(emprunts[nb_emprunts].login, login);
    nb_emprunts++;
    sauver_emprunts();
    printf("Emprunt OK.\n");
}

void rendre(char login[]) {
    printf("Id du livre a rendre : ");
    int id;
    if (scanf("%d", &id) != 1) { vider_buffer(); printf("Invalide.\n"); return; }
    vider_buffer();
    int pos = -1;
    for (int i = 0; i < nb_emprunts; i++) {
        if (emprunts[i].id_livre == id && strcmp(emprunts[i].login, login) == 0) {
            pos = i; break;
        }
    }
    if (pos == -1) { printf("Vous n'avez pas ce livre.\n"); return; }
    for (int i = pos; i < nb_emprunts - 1; i++) emprunts[i] = emprunts[i+1];
    nb_emprunts--;
    sauver_emprunts();
    printf("Livre rendu.\n");
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

void inscription() {
    if (nb_users >= MAX_USERS) { printf("Trop d'utilisateurs.\n"); return; }
    char login[30], mdp[30], role_buf[5];
    printf("Nouveau login : "); scanf("%29s", login); vider_buffer();
    if (trouver_user(login) != -1) { printf("Login deja pris.\n"); return; }
    printf("Mot de passe : "); scanf("%29s", mdp); vider_buffer();
    printf("Role ('E'/'P') : "); scanf("%4s", role_buf); vider_buffer();
    char role = role_buf[0];
    if (role >= 'a' && role <= 'z') role = role - 'a' + 'A';
    if (role != 'E' && role != 'P') { printf("Role invalide.\n"); return; }
    strcpy(users[nb_users].login, login);
    strcpy(users[nb_users].mdp, mdp);
    users[nb_users].role = role;
    nb_users++;
    sauver_users();
    printf("Compte cree.\n");
}

void menu_connecte(int idx) {
    int choix;
    while (1) {
        printf("\n=== Bonjour %s (%s) ===\n",
               users[idx].login,
               users[idx].role == 'P' ? "professeur" : "etudiant");
        afficher_mes_emprunts(users[idx].login);
        printf("1. Emprunter\n");
        printf("2. Rendre\n");
        printf("3. Voir les livres disponibles\n");
        printf("0. Deconnexion\n");
        printf("Choix : ");
        if (scanf("%d", &choix) != 1) { vider_buffer(); continue; }
        vider_buffer();
        if (choix == 0) return;
        else if (choix == 1) emprunter(users[idx].login);
        else if (choix == 2) rendre(users[idx].login);
        else if (choix == 3) afficher_livres_dispo();
        else printf("Choix invalide.\n");
    }
}

int main() {
    charger_livres();
    charger_users();
    charger_emprunts();
    printf("Charge : %d livres, %d utilisateurs, %d emprunts.\n",
           nb_livres, nb_users, nb_emprunts);

    int choix;
    while (1) {
        printf("\n=========== CY-biblioTECH ===========\n");
        printf("1. Se connecter\n");
        printf("2. Creer un compte\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        if (scanf("%d", &choix) != 1) { vider_buffer(); continue; }
        vider_buffer();
        if (choix == 0) break;
        else if (choix == 1) { int i = connexion(); if (i >= 0) menu_connecte(i); }
        else if (choix == 2) inscription();
        else printf("Choix invalide.\n");
    }
    return 0;
}
/* rendre à temps, passer le code en publique, ajouter un makefile, qui marche++, rendre readme, rapport( page garde, nom, bilan d'organisation 
des tâches, planning, les commentaires (chaque fonction), tabulations, minimum un .c un .h, programme compile, pas de changements dernière minute
ne crache pas, prof essaie de cracher le code, maîtrise du code (examen oral), sdl, personalisation*/
