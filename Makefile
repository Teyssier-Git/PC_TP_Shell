all: test_commande

GCC_OPT= -Wall -g

*.o: $^
	gcc $(GCC_OPT) -c $<

shell: shell.o readline.o commande.o
	gcc $(GCC_OPT) -o $@ $^

test_commande: test_commande.o commande.o
	gcc $(GCC_OPT) -o $@ $^

test_commande.o: commande.h commande.c test_commande.c

shell.o: shell.c readline.h commande.h

readline.o: readline.c readline.h

commande.o: commande.c commande.h


clean:
	rm -f *.o *~ shell test_commande
