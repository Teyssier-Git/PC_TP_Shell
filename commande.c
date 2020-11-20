#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include "commande.h"

// nbSep -> nombre de fois qu'on prend en compte un separatuer
// nbSep = -1 -> tous les separateurs
int separate(char **result, char *chaine, char separateur, int nbSep) {
    int size = strlen(chaine);
    int act_chaine = 0;
    int act_char = 0;

    // char ** result = (char **) malloc(N * sizeof(char **));
    // for (int i = 0; i < N; i ++)
    //   result[i] = (char*) malloc(sizeof(char) * CHAINE_LENGTH);

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
    return act_chaine +1;
}

void freeSeparate(char **result, int nb) {
    for (int i=0; i<nb; i++) {
        free(result[i]);
    }
    free(result);
}
/*
A changer, copié de dodo le sang
*/
int find_var_env (char **env, char *varName) {
  int i;
  for (i=0; env[i] != NULL; i++){
      // temporary variable containing string conresponding to the variable at the index i
      int len = strlen(env[i]);
      char * ptr = (char *) malloc(strlen(env[i])+1);
      memset(ptr,0,len);
      ptr = strncpy(ptr,env[i],len+1);

      // get the first part of the string : environment name variable
      ptr = strtok(ptr, "=");

      // if the first part coresponding to the desired variable
      // then return the index of the variable in the array
      if (strcmp(ptr,varName) == 0){
          free(ptr);
          return i;
      }else{
          free(ptr);
      }
  }
  return -1;
}

int pwd (char**envp, FILE *f) {
  int i = find_var_env(envp, "PWD");
  if (i >= 0) {
    char ** lignes = (char **) malloc(N * sizeof(char **));
    for (int i = 0; i < N; i ++)
      lignes[i] = (char*) malloc(sizeof(char) * CHAINE_LENGTH);

    int n = separate(lignes, envp[i], '=', 1);
    fprintf(f, "%s\n", lignes[1]);
    for (int i = 1; i < N ; i++)
      free(lignes[i]);

    return 0;
  }
  return 1;
}


int cd (char **envp,char *name) {
  // On découpe le chemin dest

  char ** path = (char **) malloc(N * sizeof(char **));
  for (int i = 0; i < N; i ++)
    path[i] = (char*) malloc(sizeof(char) * CHAINE_LENGTH);

  int nb_dir = separate(path, name, '/', -1);

  char ** get = (char **) malloc(N * sizeof(char **));
  for (int i = 0; i < N; i ++)
    get[i] = (char*) malloc(sizeof(char) * CHAINE_LENGTH);

  for (int act_dir = 0; act_dir < nb_dir; act_dir ++) {
    if (strcmp(path[act_dir], "..") == 0) {
      separate(get, envp[find_var_env(envp, "OLDPWD")], '=', -1);
      set(envp, "PWD", get[1]);
    } else if (strcmp(path[act_dir], "~") == 0) {
      //le ~ est forcément au début
      if (act_dir == 0) {
        separate(get, envp[find_var_env(envp, "HOME")], '=', -1);
        set(envp, "PWD", get[1]);
      } else {
        printf("bash: cd : %s no such file or directory\n", name);
      }
    } else {
      DIR* dir = opendir(path[act_dir]);
      if (dir) {
          separate(get, envp[find_var_env(envp, "PWD")], '=', -1);
          char new_pwd[CHAINE_LENGTH];
          strcpy(new_pwd, get[1]);
          new_pwd[strlen(get[1])] = '/';
          strcpy(new_pwd + strlen(get[1]) + 1, path[act_dir]);
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
  for (int i = 0; i < N; i++) {
    free(path[i]);
    free(get[i]);
  }
  return 1;
}

int set (char **env, char *name, char *var) {
    int i;
    int sizeName = strlen(name);
    char ** result = (char **) malloc(N * sizeof(char **));
    int nb = 2;
    for (int i = 0; i < N; i ++)
        result[i] = (char*) malloc(sizeof(char) * CHAINE_LENGTH);
    for (i=0; env[i]!=NULL; i++) {
        int nb = separate(result,env[i],'=', 1);
        printf(" Ok\n");

        if (nb!=2) {
            printf("Probleme avec la fonction separate\n");
            freeSeparate(result,nb);
            return -2;
        }

        if (strcmp(name,result[0])==0) {
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
            freeSeparate(result,nb);
            return 1;
        }

    }
    freeSeparate(result,nb);

    printf("Variable %s inexistante\n",name);
    return -1;
}
