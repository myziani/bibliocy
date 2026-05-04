#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===========================================================
   PARTIE I - STRUCTURES
   =========================================================== */

typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct {
    char titre[100];
    int  nb_pages;
    Date date_emprunt;
} Livre;

/* Question 3 */
Livre constructeur_livre() {
    Livre l;

    printf("Titre : ");
    fgets(l.titre, 100, stdin);
    /* supprimer le \n */
    int i = 0;
    while (l.titre[i] != '\n' && l.titre[i] != '\0') i++;
    l.titre[i] = '\0';

    printf("Nombre de pages : ");
    scanf("%d", &l.nb_pages);
    while (getchar() != '\n');

    /* livre non emprunte a la creation */
    l.date_emprunt.jour  = 0;
    l.date_emprunt.mois  = 0;
    l.date_emprunt.annee = 0;

    return l;
}

/* Question 4 */
int comparer_date(Date d1, Date d2) {
    if (d1.annee != d2.annee) {
        if (d1.annee > d2.annee) return 1;
        return 0;
    }
    if (d1.mois != d2.mois) {
        if (d1.mois > d2.mois) return 1;
        return 0;
    }
    if (d1.jour > d2.jour) return 1;
    return 0;
}

/* Question 5 */
Date ajouter_jour(Date d, int nb_jours) {
    d.jour += nb_jours;

    while (1) {
        int jours_dans_mois;
        if (d.mois % 2 == 1)
            jours_dans_mois = 31;
        else
            jours_dans_mois = 30;

        if (d.jour <= jours_dans_mois)
            break;

        d.jour -= jours_dans_mois;
        d.mois++;
        if (d.mois > 12) {
            d.mois = 1;
            d.annee++;
        }
    }

    return d;
}

/* Question 6 */
int retard(Livre l, Date date_actuelle) {
    if (l.date_emprunt.jour == 0 && l.date_emprunt.mois == 0 && l.date_emprunt.annee == 0)
        return 0;

    Date limite = ajouter_jour(l.date_emprunt, 14);
    return comparer_date(date_actuelle, limite);
}

void afficher_livres_en_retard(Livre *livres, int taille, Date date_actuelle) {
    int i;
    for (i = 0; i < taille; i++) {
        if (retard(livres[i], date_actuelle)) {
            printf("%s\n", livres[i].titre);
        }
    }
}


/* ===========================================================
   PARTIE II - ETAGERES
   =========================================================== */

/* Question 1 */
Livre *creer_etagere(int nombre_livres) {
    Livre *etagere = malloc(nombre_livres * sizeof(Livre));
    if (etagere == NULL) {
        printf("Erreur malloc\n");
        exit(1);
    }

    int i;
    for (i = 0; i < nombre_livres; i++) {
        printf("Livre %d :\n", i + 1);
        etagere[i] = constructeur_livre();
    }

    return etagere;
}

/* Question 2
   On utilise un tableau de tableaux plutot qu'un tableau 2D classique
   parce que chaque etagere peut contenir un nombre different de livres.
   Avec un tableau 2D (Livre tab[N][M]) toutes les lignes auraient la meme
   taille M ce qui gaspille de la memoire. Avec un Livre** chaque etagere
   est allouee independamment avec sa propre taille. */

/* Question 3 */
Livre **creer_bibliotheque(int *nb_etageres) {
    printf("Nombre d'etageres : ");
    scanf("%d", nb_etageres);
    while (getchar() != '\n');

    Livre **bibliotheque = malloc(*nb_etageres * sizeof(Livre *));
    if (bibliotheque == NULL) {
        printf("Erreur malloc\n");
        exit(1);
    }

    int i;
    for (i = 0; i < *nb_etageres; i++) {
        int nb;
        printf("Nombre de livres dans l'etagere %d : ", i + 1);
        scanf("%d", &nb);
        while (getchar() != '\n');
        bibliotheque[i] = creer_etagere(nb);
    }

    return bibliotheque;
}

/* Question 4 */
void verifier_poids_etageres(Livre **etageres, int nb_etageres) {
    int surpoids = 0;
    int i, j;

    for (i = 0; i < nb_etageres; i++) {
        int poids = 0;
        for (j = 0; j < 20; j++) {
            poids += etageres[i][j].nb_pages;
        }
        if (poids > 10000) {
            printf("Attention : L'etagere contenant \"%s\" va s'effondrer !\n",
                   etageres[i][0].titre);
            surpoids = 1;
        }
    }

    if (!surpoids) {
        printf("Tout est en ordre.\n");
    }
}


/* ===========================================================
   PARTIE III - FICHIERS
   =========================================================== */

/* Question 1 */
void sauvegarder_livres(Livre *livres, int taille) {
    FILE *f = fopen("bibliotheque.txt", "w");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        exit(1);
    }

    int i;
    for (i = 0; i < taille; i++) {
        fprintf(f, "%s\n", livres[i].titre);
        fprintf(f, "%d %d %d %d\n",
                livres[i].nb_pages,
                livres[i].date_emprunt.jour,
                livres[i].date_emprunt.mois,
                livres[i].date_emprunt.annee);
    }

    fclose(f);
}

/* Question 2 */
void nombre_emprunt(int *nb_empruntes, int *nb_plus_350) {
    FILE *f = fopen("bibliotheque.txt", "r");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        exit(1);
    }

    *nb_empruntes = 0;
    *nb_plus_350  = 0;

    char titre[100];
    int pages, jour, mois, annee;

    while (fgets(titre, 100, f) != NULL) {
        if (fscanf(f, "%d %d %d %d\n", &pages, &jour, &mois, &annee) != 4)
            break;

        if (jour != 0 || mois != 0 || annee != 0)
            (*nb_empruntes)++;

        if (pages > 350)
            (*nb_plus_350)++;
    }

    fclose(f);
}


/* ===========================================================
   MAIN DE TEST
   =========================================================== */
int main() {

    printf("=== Tests Partie I ===\n\n");

    Date d1 = {15, 5, 2026};
    Date d2 = {14, 5, 2026};
    Date d3 = {1,  1, 2026};

    printf("comparer_date({15,5,2026},{14,5,2026}) = %d  (attendu 1)\n", comparer_date(d1, d2));
    printf("comparer_date({1,1,2026},{1,1,2026})   = %d  (attendu 0)\n", comparer_date(d3, d3));

    Date r1 = ajouter_jour((Date){30, 4, 2024}, 2);
    printf("ajouter_jour({30,4,2024}, 2) = {%d,%d,%d}  (attendu {2,5,2024})\n", r1.jour, r1.mois, r1.annee);

    Date r2 = ajouter_jour((Date){31, 12, 2024}, 1);
    printf("ajouter_jour({31,12,2024}, 1) = {%d,%d,%d}\n", r2.jour, r2.mois, r2.annee);

    Livre livres[3];

    strcpy(livres[0].titre, "Le C en 21 jours");
    livres[0].nb_pages = 500;
    livres[0].date_emprunt = (Date){1, 3, 2026};

    strcpy(livres[1].titre, "Algorithmes");
    livres[1].nb_pages = 800;
    livres[1].date_emprunt = (Date){0, 0, 0};

    strcpy(livres[2].titre, "Linux pour les nuls");
    livres[2].nb_pages = 300;
    livres[2].date_emprunt = (Date){15, 3, 2026};

    Date aujourd_hui = {26, 3, 2026};

    printf("\nLivres en retard au 26/3/2026 :\n");
    afficher_livres_en_retard(livres, 3, aujourd_hui);

    printf("\n=== Tests Partie III ===\n\n");

    sauvegarder_livres(livres, 3);
    printf("Fichier bibliotheque.txt cree.\n");

    int nb_emp = 0, nb_350 = 0;
    nombre_emprunt(&nb_emp, &nb_350);
    printf("Livres empruntes   : %d  (attendu 2)\n", nb_emp);
    printf("Livres > 350 pages : %d  (attendu 2)\n", nb_350);

    return 0;
}
