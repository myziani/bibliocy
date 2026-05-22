/* ===========================================================
   CY-biblioTECH - Projet C preING1
   ===========================================================
   Programme qui gere une bibliotheque universitaire.
   Le code est decoupe en 4 modules :
     - utils       : petites fonctions partagees
     - livre       : les livres
     - utilisateur : les comptes (etudiants / profs)
     - emprunt     : les emprunts et les regles
   Ce fichier ne contient que le main() et les menus.
   =========================================================== */

#include <stdio.h>
#include <termios.h>      /* pour cacher les mots de passe a la saisie */
#include "livre.h"
#include "utilisateur.h"
#include "emprunt.h"

/* Petites fonctions utiles, definies en bas de ce fichier. Les autres .c
   les declarent aussi en haut de chez eux pour pouvoir les utiliser. */
void enlever_retour_ligne(char chaine[]);
void vider_buffer();
int  lire_ligne_utile(FILE *f, char buf[], int taille);
void lire_mdp(char buf[], int taille);

void menu_connecte(int idx_user) {
    while (1) {
        printf("\n=========== Bonjour %s (%s) ===========\n",
               users[idx_user].login,
               users[idx_user].role == 'P' ? "professeur" : "etudiant");
        printf("Vos emprunts :\n");
        afficher_mes_emprunts(users[idx_user].login, users[idx_user].role);

        printf("\n1. Emprunter un livre\n");
        printf("2. Rendre un livre\n");
        if (users[idx_user].role == 'P')
            printf("3. Ajouter un livre\n");
        printf("0. Deconnexion\n");
        printf("Choix : ");

        int choix;
        if (scanf("%d", &choix) != 1) { vider_buffer(); printf("Invalide.\n"); continue; }
        vider_buffer();

        if (choix == 0) {
            printf("Au revoir %s.\n", users[idx_user].login);
            return;
        }
        else if (choix == 1) emprunter(idx_user);
        else if (choix == 2) rendre(idx_user);
        else if (choix == 3 && users[idx_user].role == 'P') ajouter_livre();
        else printf("Choix invalide.\n");
    }
}

void menu_principal() {
    while (1) {
        printf("\n=========== CY-biblioTECH ===========\n");
        printf("1. Se connecter\n");
        printf("2. Creer un compte\n");
        printf("0. Quitter\n");
        printf("Choix : ");

        int choix;
        if (scanf("%d", &choix) != 1) { vider_buffer(); printf("Invalide.\n"); continue; }
        vider_buffer();

        if (choix == 0) return;
        else if (choix == 1) {
            int idx = connexion();
            if (idx >= 0) menu_connecte(idx);
        }
        else if (choix == 2) inscription();
        else printf("Choix invalide.\n");
    }
}

int main() {
    charger_livres();
    charger_users();
    charger_emprunts();

    printf("Charge : %d livres, %d utilisateurs, %d emprunts en cours.\n",
           nb_livres, nb_users, nb_emprunts);

    menu_principal();

    /* Sauvegarde finale au cas ou. */
    sauver_livres();
    sauver_users();
    sauver_emprunts();
    return 0;
}


/* ===========================================================
   PETITES FONCTIONS UTILES
   Definies ici pour eviter d'avoir un fichier separe.
   Les autres modules les declarent en haut de leur fichier .c.
   =========================================================== */

/* Enleve le \n a la fin d'une chaine lue avec fgets. */
void enlever_retour_ligne(char chaine[]) {
    int i = 0;
    while (chaine[i] != '\0') {
        if (chaine[i] == '\n' || chaine[i] == '\r') {
            chaine[i] = '\0';
            return;
        }
        i++;
    }
}

/* Vide ce qui reste sur la ligne courante (apres un scanf). */
void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* on jette les caracteres */
    }
}

/* Lit un mot de passe avec un affichage "telephone" :
     - la lettre qu'on vient de taper reste visible une fraction de seconde
       (en fait : jusqu'a la prochaine touche)
     - des qu'on tape la suivante, l'ancienne devient '*'
     - quand on appuie sur Entree, la derniere visible devient aussi '*'
   Gere la touche Suppr/Backspace pour corriger une faute de frappe.
   Si stdin n'est pas un terminal (ex : entree pipee), on lit sans masquage. */
void lire_mdp(char buf[], int taille) {
    struct termios old, new;
    int has_term = (tcgetattr(0, &old) == 0);

    if (has_term) {
        new = old;
        new.c_lflag &= ~(ECHO | ICANON);   /* pas d'echo automatique, pas de buffering ligne */
        tcsetattr(0, TCSANOW, &new);
    }

    int i = 0;
    int c;
    while (i < taille - 1) {
        c = getchar();

        /* Entree : on termine. On masque aussi la derniere lettre encore visible. */
        if (c == '\n' || c == '\r' || c == EOF) {
            if (has_term && i > 0) { printf("\b*"); fflush(stdout); }
            break;
        }

        /* Backspace (Suppr arriere) */
        if (c == 127 || c == 8) {
            if (i > 0 && has_term) {
                if (i == 1) {
                    /* Une seule lettre visible : on l'efface. */
                    printf("\b \b");
                } else {
                    /* On efface la derniere et on re-affiche l'avant-derniere
                       en clair (puisqu'elle devient la nouvelle "derniere"). */
                    printf("\b \b\b%c", buf[i-2]);
                }
                fflush(stdout);
            }
            if (i > 0) i--;
            continue;
        }

        /* Nouvelle lettre : si une lettre etait visible avant, on la masque
           en revenant en arriere et en ecrivant '*' a sa place, puis on
           affiche la nouvelle en clair. */
        if (has_term) {
            if (i > 0) printf("\b*");
            printf("%c", c);
            fflush(stdout);
        }
        buf[i++] = (char)c;
    }
    buf[i] = '\0';

    if (has_term) tcsetattr(0, TCSANOW, &old);
    printf("\n");
}

/* Lit la prochaine ligne utile d'un fichier en sautant les lignes
   vides et les commentaires (qui commencent par '#').
   Renvoie 1 si une ligne a ete lue, 0 a la fin du fichier. */
int lire_ligne_utile(FILE *f, char buf[], int taille) {
    char tampon[256];
    while (fgets(tampon, 256, f) != NULL) {
        enlever_retour_ligne(tampon);
        if (tampon[0] == '\0') continue;   /* ligne vide        */
        if (tampon[0] == '#')  continue;   /* ligne commentaire */
        int i = 0;
        while (i < taille - 1 && tampon[i] != '\0') {
            buf[i] = tampon[i];
            i++;
        }
        buf[i] = '\0';
        return 1;
    }
    return 0;
}
