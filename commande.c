#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commande.h"

// nbSep -> nombre de fois qu'on prend en compte un separatuer
// nbSep = -1 -> tous les separateurs
char **separate(int *nb, char *chaine, char separateur, int nbSep) {
    int size = strlen(chaine);
    int act_chaine = 0;
    int act_char = 0;
    char result[N][CHAINE_LENGTH];

    for (int i=0; i<size; i++) {
        if ((nbSep!=0) && chaine[i]==separateur) {
            act_chaine++;
            nbSep--;
            act_char=0;
        } else {
            result[act_chaine][act_char] = chaine[i];
            act_char++;
            result[act_chaine][act_char] = '\0';
        }
    }
    *nb = act_chaine+1;
    return result;
}

int pwd (char**envp, FILE *f) {
  for (int i=0;envp[i]!=NULL;i++) {
    if (envp[i][0] == 'P' && envp[i][1] == 'W' && envp[i][2] == 'D') {
      int j = 4;
      while (envp[i][j] != '\0') {
        fprintf(f, "%c", envp[i][j]);
        j++;
      }
      fprintf(f, "%s", "\n");
      return 0;
    }
  }
  return 1;
}

int set (char **env, char *name, char *var) {
    int i;
    int sizeName = strlen(name);
    for (i=0; env[i]!=NULL; i++) {
        printf("Alloc :");
        char **result=(char **)malloc(N*sizeof(char **));
        printf(" Ok\n");
        int nb = separate(result,env[i],'=', 2);
        if (nb!=2) {
            printf("Probleme avec la fonction separate\n");
            return -1;
        }
        printf("Maybe result : %d\n", i);
        if (strcmp(name,result[1])==0) {
            printf("Result\n");
            free(env[i]);
            env[i] = (char *)malloc(sizeof(char)*(strlen(result[0])+sizeName+strlen(var)+2));
            int k=0;
            for (; k<sizeName; k++) {
                env[i][k] = name[k];
            }
            env[i][k] = '=';
            k++;
            for (int j=0; var[j]!='\0'; j++, k++) {
                env[i][k] = var[j];
            }
            env[i][k] = '\0';
        }
        printf("free :");
        freeResult(result,N);
        printf(" Ok\n");
    }

    return 1;
}
