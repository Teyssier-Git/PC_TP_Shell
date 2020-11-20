#include <stdio.h>
#include <stdlib.h>
#include "commande.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("./test_commande <chaine> <separateur> <nbSep>\n");
        return 1;
    }

    int nb;
    char **result = separate(&nb,argv[1],argv[2][0], atoi(argv[3]));

    while (1) {
      char ** lignes = (char **) malloc(N * sizeof(char **));
      for (int i = 0; i < N; i ++)
        lignes[i] = (char*) malloc(sizeof(char) * CHAINE_LENGTH);

      int nb = separate(lignes,argv[1],argv[2][0], -1);

      for (int i=0; i<nb; i++) {
          printf("%s\n",lignes[i]);
      }

      for (int i = 0; i < N; i++)
        free(lignes[i]);
    }

    return 0;
}
