#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "commande.h"

int find_var_env (char **envp, char *nom) {
  char **ligne;
  for (int i = 0; envp[i]!=NULL; i++) {
    ligne = separate(envp[i], '=');
    if (!strcmp(envp[i], nom)) {
      return i;
    }
  }
  return -1;
}

int pwd (char**envp, FILE *f) {
  int i = find_var_env(envp, "PWD");
  if (i >= 0) {
    char **ligne = separate(envp[i], "=");
    for (int i = 0; ligne[1][i] != '\0'; i ++)
      fprintf(f, "%c", envp[i][j]);
    fprintf(f, "%s", "\n");
    return 0;
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

int cd (char **envp,char *name) {
  DIR* dir = opendir(name);
  if (dir) {
      int i = find_var_env(envp, "PWD");
      char **ligne = separate(envp[i], "=");
      char *new_pwd = (char *) malloc(sizeof(char) * (strlen(ligne[1]) + 1 + strlen(name));
      int act;
      for (act = 0; ligne[1][act] != '\0'; i++)
        new_pwd[act] = ligne[1][act];

      new_pwd[act++] = '/';
      act ++;
      for (int act_n = 0; name[act_n] != '\0'; act_n ++)
        new_pwd[act + act_n] = name[act_n];

      set(envp, "PWD", new_pwd);
      closedir(dir);
      return 0;
  } else if (ENOENT == errno) {
      printf("bash: cd: %s: No such file or directory\n", name);
  } else {
      /* opendir() failed for some other reason. */
  }
  return 1;
}
