#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include "commande.h"

// nbSep -> nombre de fois qu'on prend en compte un separatuer
// nbSep = -1 -> tous les separateurs
char ** separate(int *nb_split, char *chaine, char separateur, int nbSep) {
    int size = strlen(chaine);
    int act_chaine = 0;
    int act_char = 0;
    
    char ** result = (char **) malloc(N * sizeof(char **));
    for (int i = 0; i < N; i ++)
      result[i] = (char*) malloc(sizeof(char) * CHAINE_LENGTH);

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
    *nb_split = act_chaine +1;
    return result;
}

// int find_var_env (char **envp, char *nom) {
//   char ligne[N][CHAINE_LENGTH];
//   for (int i = 0; envp[i] != NULL; i++) {
//     separate(ligne, envp[i], '=');
//     if (strcmp(ligne[i], nom) == 0)
//       return i;
//   }
//   return -1;
// }
//
// int pwd (char**envp, FILE *f) {
//   printf("A\n");
//   int i = find_var_env(envp, "PWD");
//   printf("B\n");
//   if (i >= 0) {
//     char ligne[N][CHAINE_LENGTH];
//     separate(ligne, envp[i], '/');
//     fprintf(f, "%s\n", ligne[1]);
//     return 0;
//   }
//   return 1;
// }
//
//
// int cd (char **envp,char *name) {
//   // On découpe le chemin dest
//   char path[N][CHAINE_LENGTH];
//   int nb_dir = separate(path, name, '/');
//
//   char get[N][CHAINE_LENGTH];
//   for (int act_dir = 0; act_dir < nb_dir; act_dir ++) {
//     if (strcmp(path[act_dir], "..") == 0) {
//       separate(get, envp[find_var_env(envp, "OLDPWD")], '=');
//       set(envp, "PWD", get[1]);
//     } else if (strcmp(path[act_dir], "~") == 0) {
//       //le ~ est forcément au début
//       if (act_dir == 0) {
//         separate(get, envp[find_var_env(envp, "HOME")], '=');
//         set(envp, "PWD", get[1]);
//       } else {
//         printf("bash: cd : %s no such file or directory\n", name);
//       }
//     } else {
//       DIR* dir = opendir(path[act_dir]);
//       if (dir) {
//           separate(get, envp[find_var_env(envp, "PWD")], '=');
//           char new_pwd[CHAINE_LENGTH];
//           strcpy(new_pwd, get[1]);
//           new_pwd[strlen(get[1])] = '/';
//           strcpy(new_pwd + strlen(get[1]) + 1, path[act_dir]);
//           set(envp, "PWD", new_pwd);
//           closedir(dir);
//           return 0;
//       } else if (ENOENT == errno) {
//           printf("bash: cd: %s: No such file or directory\n", name);
//       } else {
//           /* opendir() failed for some other reason. */
//       }
//     }
//
//   }
//
//   return 1;
// }
//
// int set (char **env, char *name, char *val) {
//   return 1;
// }
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
