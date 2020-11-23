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

/*
Fonction qui libère les allocations faites dans separate
*/

void freeSeparate(char **result, int nb) {
    for (int i=0; i<nb; i++) {
        free(result[i]);
    }
    free(result);
}

/*
Cette fonction provient de Dorian BARET
Nous n'avons pas eut le temps de refaire la notre
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

int pwd (char**envp, int n) {
  if (n==0)
    printf("%s\n", getcwd(NULL, 0));
  else
    printf("%s", getcwd(NULL, 0));
  return 0;
}


int cd (char **envp, char *name) {
  int nb_oldPWD;
  //On récupère le PWD actuelle
  char **oldPWD = separate(&nb_oldPWD, envp[find_var_env(envp, "PWD")], "=", -1);
  if (strcmp(name, "~") == 0) {
    /* On replace le ~ lue par le repertoire home qu'on récupère dans l'environnement
    i.e. name = home
    */
    int nb_tmp;
    char **tmp = separate(&nb_tmp, envp[find_var_env(envp, "HOME")], "=", -1);
    strcpy(name,tmp[1]);
    freeSeparate(tmp, nb_tmp);
  }

  if (!chdir(name)) {
    /*Le chemin spécifié existe
    La commande chdir a mis a jour le workind directory
    */
    char *newPWD = getcwd(NULL, 0); // on récupère le nouveau PWD
    // on met a jour l'environnement
    set(envp, "PWD", newPWD);
    set(envp, "OLDPWD", oldPWD[1]);
    free(newPWD);
  } else {
    // le fichier spécifié n'était pas correct
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


int print (char**envp, char *name) {
    if (name == NULL) {
        for (int i=0; envp[i]!=NULL; i++) {
            printf("%s\n",envp[i]);
        }
        return 0;
    }
    int i = find_var_env(envp, name);
    if (i >= 0) {
        int nb;
        char **lignes = separate(&nb, envp[i], "=", -1);

        printf("%s\n", lignes[1]);

        freeSeparate(lignes, nb);
        return 0;
    }
    return 1;
}


void zombie_cleaner() {
  int i_got_a_zombie = 1;
  int deadID;
  while(i_got_a_zombie) {
    deadID = waitpid(-1, NULL, WNOHANG);
    if(deadID > 0) {
      i_got_a_zombie = 1;
    } else if (deadID == 0) {
      i_got_a_zombie = 0;
    } else {
      if (errno == ECHILD) {
        return; // no children
      } else {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }
    }
  }
}

typedef struct ins_ {
  char *cmd; //commande
  char **args; //arguments, avec la commande en premier
  int input;
  int output;
  /*
  red = 0 normal
  red = 1 sortie redirigées vers fichier avec écrasement >
  red = 2 sortie redirigées vers fichier sans écrasement >>
  */
  int red;
  struct ins_ *ins_suiv;
} ins;

#define	READ_END	0
#define	WRITE_END	1

int execCommands(char**envp, char **words, int do_in_background) {
    int i = 0;
    ins *head = (ins *)malloc(sizeof(ins));
    int fd[2];

    ins *actLis = head;
    ins *tmp_free = head;
    //Par défaut, on attend l'execusion du CHILD
    head->input = STDIN_FILENO;
    int end = 0;
    // On créé l'instruction
    while ((end != 1) && (words[i] != NULL)) {
      //  on recupère la commande et ses arguments
        actLis->cmd = words[i];
        actLis->args = words + i;
        actLis->red = 0;
        char *fname = NULL;
        while ((words[i] != NULL) && strcmp(words[i], "|") != 0) {
            if (strcmp(words[i], ">") == 0) {
              words[i] = NULL;
              i++;
              if (words[i] != NULL) {
                if (strcmp(words[i], ">") == 0) {
                  actLis->red = 2;
                  words[i] = NULL;
                  i++;
                  fname = words[i];
              } else {
                  actLis->red = 1;
                  fname = words[i];
                }
              } else {
                printf("bash syntax error\n");
                return 1;
              }
          } else if (strcmp(words[i], "<") == 0) {
              actLis->red = 3;
              words[i] = NULL;
              i++;
              fname = words[i];
          }
            i++;
        }

        if (words[i] != NULL) {
            // on a un lu un | donc on set le | a null
            words[i] = NULL;
            // on cré l'instruction suivante
            actLis->ins_suiv = (ins *) malloc(sizeof(ins));
            // on set le pipe entre l'output de la commande actuelle avec l'input de la commande suivante
            pipe(fd);
            actLis->output = fd[WRITE_END];
            actLis->ins_suiv->input = fd[READ_END];

            if (actLis->red == 1) {
                close(actLis->output);
                actLis->output = fileno(fopen(fname,"w"));
            } else if (actLis->red == 2) {
                close(actLis->output);
                actLis->output = fileno(fopen(fname,"a"));
            } else if (actLis->red == 3) {
                close(actLis->input);
                actLis->input = fileno(fopen(fname,"r"));
            }

            //on passe a la commande suivante
            actLis = actLis->ins_suiv;
        } else {
            // on est arrivé à la fin i.e. NULL
            actLis->ins_suiv = NULL;
            actLis->output = STDOUT_FILENO;
            if (actLis->red == 1) {
                actLis->output = fileno(fopen(fname,"w"));
            } else if (actLis->red == 2) {
                actLis->output = fileno(fopen(fname,"a"));
            } else if (actLis->red == 3) {
                actLis->input = fileno(fopen(fname,"r"));
            }
            end = 1;
        }
        i++;
    }
    // on execute si l'instruction interne
    if (0==strcmp(head->cmd, "pwd"))
        pwd(envp,0);
    else if (0==strcmp(head->cmd, "print"))
        print(envp,head->args[1]);
    else if (0==strcmp(head->cmd, "set")) {
      if (head->args[1] != NULL && head->args[2] != NULL)
        set(envp,head->args[1],head->args[2]);
      else
        printf("Commande set incorrect\n");
    }
    else if (0==strcmp(head->cmd, "cd"))
        cd(envp,head->args[1]);
    else if (0==strcmp(head->cmd, "env"))
        print(envp,NULL);
    else { //l'instruction est externe
      int childPID;
      while (head != NULL) {
          childPID = fork();

          if (childPID < 0) { /* Error */
              exit(-1);
          }
          if (childPID == 0) {/* Child Code */
              // On connect les pipes input et output de la commande actuelle sur SDTIN et SDTOUT
              if (head->input != STDIN_FILENO) {
                dup2(head->input, STDIN_FILENO);
                close(head->input);
              }
              if (head->output != STDOUT_FILENO) {
                dup2(head->output, STDOUT_FILENO);
                close(head->output);
              }

              //On rajoute les /bin/ davant la commande a executer
              char comm[6+strlen(head->cmd)];
              char tmp[] = "/bin/";
              for (int i=0; i<7; i++) {
                  comm[i] = tmp[i];
              }
              strcat(comm,head->cmd);

              // on vérifie si la commande existe
              FILE *file;
              if (file = fopen(comm, "r")){
                  fclose(file);
                  //on execute la commande avec les arguments
                  execve(comm, head->args, envp);
              } else {
                  printf("Commande missing\n");
                  return 1;
              }
          } else { /* Parent Code */
            // On ferme les pipes, sauf si c'est STDIN et STDOUT
              if (head->input != STDIN_FILENO) {
                  close(head->input);
              }
              if (head->output != STDOUT_FILENO) {
                  close(head->output);
              }

              //on passe à l'instruction suivantes
              head = head->ins_suiv;
          }
      }

      // on Attend l'execution des fils
      int status;
      if (-1==waitpid(childPID, &status, do_in_background)) {
          perror("waitpid: ");
          return -1;
      }
    }


    // Free la liste des instructions
    ins *tmp_free_suiv;
    while (tmp_free != NULL){
      tmp_free_suiv = tmp_free->ins_suiv;
      free(tmp_free);
      tmp_free = tmp_free_suiv;
    }
    free(tmp_free);
    return 1;
}
