#ifndef COMMANDE_H
#define COMMANDE_H
#include <stdio.h>
#define N 512
#define CHAINE_LENGTH 128

char **  separate(int* nb_split, char *chaine, char separateur);

void ls (void);
int cd (char **envp,char *name);
void cp(void);
int pwd (char**envp, FILE *f);
void print (void);
int set (char **env, char *name, char *val);

#endif
