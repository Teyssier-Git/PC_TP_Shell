#ifndef COMMANDE_H
#define COMMANDE_H


void ls (void);
void cd (void);
void cp(void);
int pwd (char**envp, FILE *f);
void print (void);
int set (char**envp, FILE f);

#endif
