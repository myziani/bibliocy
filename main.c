#include <stdio.h>
#include "livre.h"
#include "utilisateur.h"
#include "emprunt.h"

/* Les autres .c ont juste besoin de connaitre les prototypes. */
void enlever_retour_ligne(char chaine[]);
void vider_buffer();

void menu_connecte(int idx) {
    int choix;
    while (1) {
        printf("\n=== Bonjour %s (%s) ===\n",
               users[idx].login,
               users[idx].role == 'P' ? "professeur" : "etudiant");
        printf("Vos emprunts :\n");
        afficher_mes_emprunts(users[idx].login, users[idx].role);

        printf("1. Emprunter\n2. Rendre\n");
        if (users[idx].role == 'P') printf("3. Ajouter un livre\n");
        printf("0. Deconnexion\nChoix : ");
        if (scanf("%d", &choix) != 1) { vider_buffer(); continue; }
        vider_buffer();
        if (choix == 0) return;
        else if (choix == 1) emprunter(idx);
        else if (choix == 2) rendre(idx);
        else if (choix == 3 && users[idx].role == 'P') ajouter_livre();
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
        printf("1. Se connecter\n2. Creer un compte\n0. Quitter\nChoix : ");
        if (scanf("%d", &choix) != 1) { vider_buffer(); continue; }
        vider_buffer();
        if (choix == 0) break;
        else if (choix == 1) { int i = connexion(); if (i >= 0) menu_connecte(i); }
        else if (choix == 2) inscription();
        else printf("Choix invalide.\n");
    }

    sauver_livres();
    sauver_users();
    sauver_emprunts();
    return 0;
}

/* ===========================================================
   Petites fonctions utiles (utilisees aussi par les autres .c).
   =========================================================== */

void enlever_retour_ligne(char chaine[]) {
    int i = 0;
    while (chaine[i] != '\0') {
        if (chaine[i] == '\n' || chaine[i] == '\r') { chaine[i] = '\0'; return; }
        i++;
    }
}

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

