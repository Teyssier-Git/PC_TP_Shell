#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"
#include "commande.h"

int main(int argc, char** argv, char**envp) {

    // Recuperation taille envp
    int tmp_size=0;
    while (envp[tmp_size]!=NULL) {
        tmp_size++;
    }

    // printf("%d\n",tmp_size);
    char **m_env = (char **)malloc(tmp_size*sizeof(char **));
    for (int i=0;i<tmp_size;i++) {
        m_env[i] = (char *)malloc(strlen(envp[i])*sizeof(char));
        strcpy(m_env[i],envp[i]);
    }

    // affichage environement
    for (int i=0;i<tmp_size;i++) {
        printf("env[%d]=%s\n",i,m_env[i]);
    }
    printf("\n");

    // set stdout without buffering so what is printed
    // is printed immediately on the screen.
    // setvbuf(stdout, NULL, _IONBF, 0);
    // setbuf(stdout, NULL);

  for (;;) {
    printf("> ");
    fflush(stdout);
    char *line = readline();
    printf("%s\n", line);
    char** words = split_in_words(line);

    /* Execution */
    if (!strcmp(words[0], "pwd"))
      pwd(m_env, stdout);

    free(words);
    free(line);
    
  }
  return 0;
}
