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
  int nb_oldPWD;
  char **oldPWD = separate(&nb_oldPWD, envp[find_var_env(envp, "PWD")], "=", -1);
  if (strcmp(name, "~") == 0) {
    int nb_tmp;
    char **tmp = separate(&nb_tmp, envp[find_var_env(envp, "HOME")], "=", -1);
    strcpy(name,tmp[1]);
    freeSeparate(tmp, nb_tmp);
  }
  if (!chdir(name)) {
    char *newPWD = getcwd(NULL, 0);
    set(envp, "PWD", newPWD);
    set(envp, "OLDPWD", oldPWD[1]);
    free(newPWD);
  } else {
    printf("BASH : cd : %s no such file or firectory\n", name);
  }
  freeSeparate(oldPWD, nb_oldPWD);
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

int externCommands(char**envp, char **words) {
    int pid = fork();
    int status;

    int opt = 0;
    int i = 0;
    while (words[i]!=NULL)
      i++;

    if (words[i-1][0] == '&') {
      opt = WNOHANG;
      words[i-1] = NULL;
    }


    if (pid == -1) {
        perror("fork : ");
        return -1;
    }
    if (pid == 0) {
        /* transforme "..." -> ... */
        for (int i=0; words[i]!=NULL; i++) {
            if (words[i][0] == '"') {
                int size = strlen(words[i]);
                if (words[i][size-1] == '"') {
                    int k=0;
                    for (; k<size-2; k++) {
                        words[i][k] = words[i][k+1];
                    }
                    words[i][k] = '\0';
                }
            }
        }

        /* créé /bin/... */
        char comm[6+strlen(words[0])];
        char tmp[] = "/bin/";
        for (int i=0; i<7; i++) {
            comm[i] = tmp[i];
        }
        strcat(comm,words[0]);

        execve(comm, words, envp);
        return 0;
    }
    if (-1==waitpid(pid, &status,opt)) {
        perror("waitpid: ");
        return -1;
    }
    return 0;
}
