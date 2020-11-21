#ifndef COMMANDE_H
#define COMMANDE_H
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 128
#define CHAINE_LENGTH 512

char ** separate(int *bn, char *chaine, char *sep, int nb_sep) ;

int cd (char **envp,char *name);
void cp(void);
int pwd (char**envp, FILE *f, int n);
int print (char**envp, char *name, int n, FILE *f);
int set (char **env, char *name, char *val);
int externCommands(char**envp, char **words);
int execCommands(char**envp, char **words);

#endif
