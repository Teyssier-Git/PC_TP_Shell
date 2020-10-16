#ifndef COMMANDE_H
#define COMMANDE_H


void ls (void);
int cd (char **envp,char *name);
void cp(void);
int pwd (char**envp, FILE *f);
void print (void);
int set (char**envp, FILE f);

#endif
