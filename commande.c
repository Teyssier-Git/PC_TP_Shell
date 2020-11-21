#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commande.h"

/*

fonction qui sépare la chaine de charactère passées en arguments (chaine) en focntion du séparateur "sep"
retourn un tableau contenant les éléments séparés

*/
char ** separate(int *bn, char *chaine, char *sep, int nb_sep) {
  /*
  On copie la chaine passé en arguments
  */
  char *cchaine = (char * ) malloc(sizeof(char) * CHAINE_LENGTH);
  strcpy(cchaine, chaine);

  /*
  On compte le nombre d'éléments séparés dans la chaine
  */
  int nTotalItems = 0;
  char *buffer = strtok(cchaine, sep);
  int tmb_sep = nb_sep;

  do {
    nTotalItems ++;
    nb_sep --;
  } while ((nb_sep !=0) && ((buffer=strtok(NULL, sep))!= NULL));

  /*
  On réinitialise le buffer
  */
  char** strArray = malloc( nTotalItems * sizeof( char* ));
  int    nIndex  = 0;

  strcpy(cchaine, chaine);
  // re-setup buffer
  buffer = strtok(cchaine, sep);
  nb_sep = tmb_sep;

  do {
     // allocate the buffer for string and copy...
     strArray[ nIndex ] = malloc( strlen( buffer ) + 1 );
     strcpy( strArray[ nIndex ], buffer );
     nb_sep --;
     nIndex++;
   } while ((nb_sep !=0) && ((buffer=strtok(NULL, sep))!= NULL));

  *bn = nTotalItems;
  free(cchaine);
  return strArray;
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

int pwd (char**envp, FILE *f, int n) {
  int i = find_var_env(envp, "PWD");
  if (i >= 0) {
    int nb;
    char **lignes = separate(&nb, envp[i], "=", -1);

    fprintf(f, "%s", lignes[1]);
    if (n==0)
      fprintf(f, "%s", "\n");

    freeSeparate(lignes, nb);
    return 0;
  }
  return 1;
}




int cd (char **envp, char *name) {
  // On découpe le chemin dest
  int nb_dir;
  char ** path = separate(&nb_dir, name, "/", -1);

  for (int act_dir = 0; act_dir < nb_dir; act_dir ++) {
    //on lit un ..
    if (strcmp(path[act_dir], "..") == 0) {
      int nget;
      int tmp;
      /*
      On récupere le PWD et on le découpe
      */
      char ** get = separate(&nget, separate(&tmp, envp[find_var_env(envp, "PWD")], "=", -1)[1], "/", -1);
      int n = 0;
      for (int k = 0; k < nget-1; k++)
        n+=strlen(get[k]) + 1;
      /*
      On reconstruit le new PWD en enlevant le dernier élément de l'ancien PWD
      */
      char newPWD[n];
      char act = 0;
      for (int k = 0; k < nget-1; k++) {
        newPWD[act] = '/';
        act++;
        strcpy(newPWD + act, get[k]);
        act += strlen(get[k]);
      }
      newPWD[act] = '\0';
      //On set le new PWD
      set(envp, "PWD", newPWD);
      freeSeparate(get, nget);

    } else if (strcmp(path[act_dir], "~") == 0) {
      //le ~ est forcément au début
      if (act_dir == 0) {
        int nget;
        char ** get = separate(&nget, envp[find_var_env(envp, "HOME")], "=", -1);
        char t[CHAINE_LENGTH];
        strcpy(t, get[1]);
        freeSeparate(get, nget);
        set(envp, "PWD", t);
      } else {
        //On a lu un ~ au milieu du chemin donné, erreur
        printf("bash: cd : %s no such file or directory\n", name);
      }
    } else {
      // On tente d'ouvrir le chemin du dossier
      int nget;
      char ** get = separate(&nget, envp[find_var_env(envp, "PWD")], "=", -1);
      char new_pwd[CHAINE_LENGTH];
      strcpy(new_pwd, get[1]);
      new_pwd[strlen(get[1])] = '/';
      strcpy(new_pwd + strlen(get[1]) + 1, path[act_dir]);
      freeSeparate(get, nget);

      DIR* dir = opendir(new_pwd);
      if (dir != NULL) {
          set(envp, "PWD", new_pwd);
          closedir(dir);
      } else if (ENOENT == errno) {
          printf("bash Dir: cd: %s: No such file or directory\n", new_pwd);
      } else {
          /* opendir() failed for some other reason. */
      }
    }

  }
  freeSeparate(path, nb_dir);
  return 1;
}

int set (char **env, char *name, char *var) {
    int i;
    int sizeName = strlen(name);
    for (i=0; env[i]!=NULL; i++) {

        char tmp[strlen(env[i])];;
        strcpy(tmp, env[i]);
        char *p1 = strtok(tmp, "=");

        if (strcmp(name,p1)==0) {
            free(env[i]);
            env[i] = (char *)malloc(sizeof(char)*(strlen(p1)+sizeName+strlen(var)+2));

            int k=0;
            for (; k<sizeName; k++) {
                env[i][k] = p1[k];
            }
            env[i][k] = '=';
            k++;
            for (int j=0; var[j]!='\0'; j++, k++) {
                env[i][k] = var[j];
            }
            env[i][k] = '\0';
            return 1;
        }
    }

    printf("Variable %s inexistante\n",name);
    return -1;
}


int print (char**envp, char *name, int n, FILE *f) {
      int i = find_var_env(envp, name);
      if (i >= 0) {
        int nb;
        char **lignes = separate(&nb, envp[i], "=", -1);

        fprintf(f, "%s", lignes[1]);
        if (n==0)
          fprintf(f, "%s", "\n");

        freeSeparate(lignes, nb);
        return 0;
      }
      return 1;
}

int cat(char**envp, char **words) {
    int pid = fork();
    int status;
    switch (pid) {
        case -1:
            perror("fork : cat");
            return -1;
            break;
        case 0:
            execve("/bin/cat", words, envp);
            break;
        default:
            if (-1==waitpid(pid, &status,0))
                perror("waitpid: cat");
            break;
        }
    return 0;
}
