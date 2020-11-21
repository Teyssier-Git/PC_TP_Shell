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
    pwd(m_env, stdout, 1);
    printf("> ");
    fflush(stdout);
    char *line;
    char** words;
    do {
      line = readline();
      words = split_in_words(line);
    } while(words[0] == NULL);
    /* Execution */
    if (0==strcmp(words[0], "pwd"))
        pwd(m_env, stdout,0);
    else if (0==strcmp(words[0], "print"))
        print(m_env,words[1],0,stdout);
    else if (0==strcmp(words[0], "set"))
        set(m_env,words[1],words[2]);
    else if (0==strcmp(words[0], "cd"))
        cd(m_env,words[1]);
    else if (0==strcmp(words[0], "env")) {
        for (int i=0;i<tmp_size;i++) {
            printf("env[%d]=%s\n",i,m_env[i]);
        }
        printf("\n");
    } else {
        execCommands(m_env,words);
    }


    free(words);
    free(line);

  }
  return 0;
}
