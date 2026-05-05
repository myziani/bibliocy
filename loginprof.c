#include<stdio.h>
#include<string.>
int main(){
  char identifiant [100];
  char mdp [100];
  log_f[100];
  mdp_f[100];
printf("vous commencer la connexion");
print("saisir votre identifiant");
  scanf(%s,identifiant);
  printf("saisir votre mdp");
  scanf(%s,mdp);
  FILE *f = fopen("utilisateurs.txt", "r");
    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        return 1;
    }
  while (fscanf(f, "%s %s", log_f, mdp_f) != EOF) {
        if (strcmp(id, log_f) == 0 && strcmp(mdp, mdp_f) == 0) {
          printf("connexion reussie");
        }
      else {
            printf("erreur");
    }
    fclose(f);
    return 0;
}
