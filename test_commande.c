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

    for (int i=0; i<nb; i++) {
        printf("%s\n",result[i]);
    }

    return 0;
}
