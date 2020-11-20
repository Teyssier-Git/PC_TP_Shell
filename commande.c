#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
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
  char **path = separate(name, '/');
  for (int act_dir = 0; path[act_dir] != NULL; act_dir ++) {
    if (path[act_dir] == "..") {

    } else if (path[act_dir] == "~") {
      int indice_pwd = find_var_env(envp, "PWD");
      char *old _pwd= separate(envp[indice_pwd], "=")[1];
    } else {
      DIR* dir = opendir(path[act_dir]);
      if (dir) {
          int indice_pwd = find_var_env(envp, "PWD");
          char *old _pwd= separate(envp[indice_pwd], "=")[1];
          char *new_pwd = (char *) malloc(sizeof(char) * (strlen(old_pwd) + 1 + strlen(path[act_dir]));
          int size_old_pwd = strlen(old_pwd);
          new_pwd = strncpy (new_pwd,old_pwd, size_old_pwd);
          new_pwd = strncpy (new_pwd + size_old_pwd ,"/", 1);
          new_pwd = strncpy (new_pwd + size_old_pwd + 1,path[act_dir], strlen(path[act_dir]));
          set(envp, "PWD", new_pwd);
          closedir(dir);
          return 0;
      } else if (ENOENT == errno) {
          printf("bash: cd: %s: No such file or directory\n", name);
      } else {
          /* opendir() failed for some other reason. */
      }
    }

  }

  return 1;
}
