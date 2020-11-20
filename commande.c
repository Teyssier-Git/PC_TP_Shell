#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commande.h"

int separate(char *result[], char *chaine, char separateur) {
    int size = strlen(chaine);
    int act_chaine = 0;
    int act_char = 0;

    for (int i = 0; i < N; i ++)
      result[i] = (char *)malloc(sizeof(char)*CHAINE_LENGTH);

    for (int i=0; i<size; i++) {
        if (chaine[i] == separateur) {
            act_chaine++;
            act_char=0;
        } else {
            result[act_chaine][act_char] = chaine[i];
            act_char++;
            result[act_chaine][act_char] = '\0';
        }
    }
    return act_chaine+1;
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

// int set (char **env, char *name, char *var) {
//     int i;
//     int sizeName = strlen(name);
//     for (i=0; env[i]!=NULL; i++) {
//         for (int j=0; envp[i][j] != '=' && envp[i][j] != '\0'; j++) {
//
//         }
//         if (strcmp(name,tmp)) {
//
//         }
//     }
//
//     printf("\n");
//
// }
