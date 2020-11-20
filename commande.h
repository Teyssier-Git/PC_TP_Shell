#ifndef COMMANDE_H
#define COMMANDE_H
#include <stdio.h>

int separate(char **result, char *chaine, char separateur);

void ls (void);
void cd (void);
void cp(void);
int pwd (char**envp, FILE *f);
void print (void);
int set (char **env, char *name, char *val);

#endif
