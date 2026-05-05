 #include<stdio.h>
#include<string.>
int main(){
  char identifiant [100];
  char mdp [100];
FILE* f;
  printf("vous commencer l'inscription");
  print("choisir un identifiant");
  scanf(%s,identifiant);
  printf("saisir votre mdp");
  scanf(%s,mdp);
  f =fopen("prof.txt","a");
  if(f==null){
        exit(1);
  }
fprintf(f,"%s,%s\n",identifiant,mdp);
fclose(f);
printf("inscription réussite);
  return 0;
  }
  
