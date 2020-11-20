#ifndef COMMANDE_H
#define COMMANDE_H
#include <stdio.h>
#define N 128
#define CHAINE_LENGTH 512

char ** separate(int *bn, char *chaine, char *sep, int nb_sep) ;

void ls (void);
int cd (char **envp,char *name);
void cp(void);
int pwd (char**envp, FILE *f, int n);
int print (char**envp, char *name, FILE *f);
int set (char **env, char *name, char *val);

#endif
