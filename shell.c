#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "readline.h"

int main(int argc, char** argv, char**envp) {

  for (int i=0;envp[i]!=NULL;i++)
    printf("env[%d]=%s\n",i,envp[i]);
  printf("\n");

  // set stdout without buffering so what is printed
  // is printed immediately on the screen.
  // setvbuf(stdout, NULL, _IONBF, 0);
  // setbuf(stdout, NULL);

  for (;;) {
    printf("> ");
    fflush(stdout);
    char* line = readline();
    printf("%s\n", line);
    char** words = split_in_words(line);
    for (int i=0;words[i]!=NULL;i++)
      printf("[%s], ", words[i]);
    printf("\n");
    free(words);
    free(line);
  }
  return 0;
}
