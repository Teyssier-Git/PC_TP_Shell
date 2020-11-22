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
    char **m_env = (char **)malloc((tmp_size+1)*sizeof(char **));
    for (int i=0;i<tmp_size;i++) {
        m_env[i] = (char *)malloc(strlen(envp[i])*sizeof(char));
        strcpy(m_env[i],envp[i]);
    }
    m_env[tmp_size] = NULL;

    // set stdout without buffering so what is printed
    // is printed immediately on the screen.
    // setvbuf(stdout, NULL, _IONBF, 0);
    // setbuf(stdout, NULL);

  for (;;) {
    zombie_cleaner();
    char *line;
    char** words;
    do {
      pwd(m_env, 1);
      printf("> ");
      fflush(stdout);
      line = readline();
      words = split_in_words(line);
    } while(words[0] == NULL);
    /* Execution */

    int nb_words = 0;
    while (words[nb_words] != NULL)
      nb_words++;

    int do_in_background = 0;
    if (strcmp(words[nb_words - 1], "&") == 0) {
      do_in_background =WNOHANG;
      words[nb_words -1 ] = NULL;
    }


    execCommands(m_env,words, do_in_background);

    for (int i = 0; i < nb_words; i++) {
        free(words[i]);
    }
    free(words);
    free(line);

  }
  return 0;
}
