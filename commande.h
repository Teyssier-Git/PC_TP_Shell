#ifndef COMMANDE_H
#define COMMANDE_H
#include <stdio.h>
#define N 512
#define CHAINE_LENGTH 128

char **separate(int *nb, char *chaine, char separateur, int nbSep);

void ls (void);
void cd (void);
void cp(void);
int pwd (char**envp, FILE *f);
void print (void);
int set (char **env, char *name, char *val);

#endif
