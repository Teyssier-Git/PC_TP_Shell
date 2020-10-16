#include <stdio.h>
#include <string.h>
#include "commande.h"

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
/*
int set (char* name, char**envp) {
    for (int i=0;envp[i]!=NULL;i++) {
        int
        char *tmp[];
        for (int j=0; envp[i][j] != '='; j++) {

        }
        if (strcmp(name,tmp)) {

        }
    }

    printf("\n");

}
*/
