/* emprunt.c - Implementation des emprunts et des regles de la bibliotheque. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "emprunt.h"
#include "livre.h"
#include "utilisateur.h"

/* Fonctions utiles definies dans main.c */
void enlever_retour_ligne(char chaine[]);
void vider_buffer();
int  lire_ligne_utile(FILE *f, char buf[], int taille);

Emprunt emprunts[MAX_EMPRUNTS];
int     nb_emprunts = 0;

void charger_emprunts() {
    FILE *f = fopen("loans.txt", "r");
    if (f == NULL) return;

    nb_emprunts = 0;
    char ligne[40];
    while (nb_emprunts < MAX_EMPRUNTS) {
        if (!lire_ligne_utile(f, ligne, 40)) break;
        emprunts[nb_emprunts].id_livre = atoi(ligne);
        if (!lire_ligne_utile(f, emprunts[nb_emprunts].login, 30)) break;
        if (!lire_ligne_utile(f, ligne, 40)) break;
        emprunts[nb_emprunts].heure_emprunt = atol(ligne);
        nb_emprunts++;
    }
    fclose(f);
}

void sauver_emprunts() {
    FILE *f = fopen("loans.txt", "w");
    if (f == NULL) return;
    fprintf(f, "# CY-biblioTECH - Emprunts en cours\n");
    fprintf(f, "# 3 lignes par emprunt : id du livre, login, heure d'emprunt (timestamp Unix)\n");
    fprintf(f, "\n");
    for (int i = 0; i < nb_emprunts; i++) {
        fprintf(f, "# --- %s a emprunte le livre #%d ---\n",
                emprunts[i].login, emprunts[i].id_livre);
        fprintf(f, "%d\n",  emprunts[i].id_livre);
        fprintf(f, "%s\n",  emprunts[i].login);
        fprintf(f, "%ld\n", emprunts[i].heure_emprunt);
        fprintf(f, "\n");
    }
    fclose(f);
}

int livre_emprunte(int id) {
    for (int i = 0; i < nb_emprunts; i++) {
        if (emprunts[i].id_livre == id) return 1;
    }
    return 0;
}

int compter_emprunts_user(char login[]) {
    int n = 0;
    for (int i = 0; i < nb_emprunts; i++) {
        if (strcmp(emprunts[i].login, login) == 0) n++;
    }
    return n;
}

int duree_pour(char role) {
    if (role == 'P') return DUREE_PROF;
    return DUREE_ETU;
}

int a_des_retards(char login[], char role) {
    long maintenant = time(NULL);
    int  duree      = duree_pour(role);
    for (int i = 0; i < nb_emprunts; i++) {
        if (strcmp(emprunts[i].login, login) == 0) {
            if (emprunts[i].heure_emprunt + duree < maintenant) return 1;
        }
    }
    return 0;
}

void afficher_mes_emprunts(char login[], char role) {
    long maintenant = time(NULL);
    int  duree      = duree_pour(role);

    int n = 0;
    for (int i = 0; i < nb_emprunts; i++) {
        if (strcmp(emprunts[i].login, login) != 0) continue;

        int   idx   = trouver_livre(emprunts[i].id_livre);
        char *titre = (idx >= 0) ? livres[idx].titre : "(inconnu)";

        long fin     = emprunts[i].heure_emprunt + duree;
        long restant = fin - maintenant;

        if (restant < 0)
            printf("  [#%d] %-25s  *** EN RETARD de %ld secondes ***\n",
                   emprunts[i].id_livre, titre, -restant);
        else
            printf("  [#%d] %-25s  a rendre dans %ld secondes\n",
                   emprunts[i].id_livre, titre, restant);
        n++;
    }
    if (n == 0) printf("  (aucun livre emprunte)\n");
}

void emprunter(int idx_user) {
    char *login = users[idx_user].login;
    char  role  = users[idx_user].role;

    /* 1) Verifier les regles avant de demander quoi que ce soit. */
    if (a_des_retards(login, role)) {
        printf("Vous avez des livres en retard, rendez-les d'abord.\n");
        return;
    }
    int max = (role == 'P') ? MAX_PROF : MAX_ETUDIANT;
    if (compter_emprunts_user(login) >= max) {
        printf("Limite atteinte (%d livres maximum pour vous).\n", max);
        return;
    }
    if (nb_emprunts >= MAX_EMPRUNTS) {
        printf("Trop d'emprunts dans le systeme.\n");
        return;
    }

    /* 2) Tri / filtre. */
    printf("\n1. Trier par titre\n");
    printf("2. Trier par auteur\n");
    printf("3. Filtrer par categorie\n");
    printf("Choix : ");
    int choix;
    if (scanf("%d", &choix) != 1) { vider_buffer(); printf("Invalide.\n"); return; }
    vider_buffer();

    char filtre_cat[40] = "";
    if (choix == 1)      trier_livres('t');
    else if (choix == 2) trier_livres('a');
    else if (choix == 3) {
        printf("Categorie a chercher : ");
        fgets(filtre_cat, 40, stdin);
        enlever_retour_ligne(filtre_cat);
        trier_livres('c');
    } else {
        printf("Choix invalide.\n");
        return;
    }
    sauver_livres();

    /* 3) Afficher les livres disponibles. */
    printf("\nLivres disponibles :\n");
    int vu = 0;
    for (int i = 0; i < nb_livres; i++) {
        if (livre_emprunte(livres[i].id)) continue;
        if (choix == 3 && strcmp(livres[i].categorie, filtre_cat) != 0) continue;
        afficher_livre(i);
        vu++;
    }
    if (vu == 0) { printf("  (aucun livre disponible)\n"); return; }

    /* 4) Demander quel livre emprunter. */
    printf("\nId du livre a emprunter (0 pour annuler) : ");
    int id;
    if (scanf("%d", &id) != 1) { vider_buffer(); printf("Invalide.\n"); return; }
    vider_buffer();
    if (id == 0) return;

    if (trouver_livre(id) == -1) { printf("Ce livre n'existe pas.\n"); return; }
    if (livre_emprunte(id))      { printf("Deja emprunte par quelqu'un d'autre.\n"); return; }

    emprunts[nb_emprunts].id_livre      = id;
    strcpy(emprunts[nb_emprunts].login, login);
    emprunts[nb_emprunts].heure_emprunt = time(NULL);
    nb_emprunts++;
    sauver_emprunts();
    printf("Emprunt enregistre. Bonne lecture !\n");
}

void rendre(int idx_user) {
    char *login = users[idx_user].login;

    printf("Id du livre a rendre : ");
    int id;
    if (scanf("%d", &id) != 1) { vider_buffer(); printf("Invalide.\n"); return; }
    vider_buffer();

    int pos = -1;
    for (int i = 0; i < nb_emprunts; i++) {
        if (emprunts[i].id_livre == id &&
            strcmp(emprunts[i].login, login) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("Vous n'avez pas emprunte ce livre.\n");
        return;
    }

    /* On decale tous les emprunts suivants d'une case vers la gauche. */
    for (int i = pos; i < nb_emprunts - 1; i++) {
        emprunts[i] = emprunts[i+1];
    }
    nb_emprunts--;
    sauver_emprunts();
    printf("Livre rendu. Merci !\n");
}
