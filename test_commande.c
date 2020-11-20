#include <stdio.h>
#include <stdlib.h>
#include "commande.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("./test_commande <chaine> <separateur>\n");
        return 1;
    }

    char **result = (char **)malloc(sizeof(char **));
    int nb = separate(result,argv[1],argv[2][0]);

    printf("%d\n",nb);
    for (int i=0; i<nb; i++) {
        printf("%s\n",result[i]);
    }

    return 0;
}
