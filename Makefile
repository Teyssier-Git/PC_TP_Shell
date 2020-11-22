all: shell

GCC_OPT= -Wall -g

*.o: $^
	gcc $(GCC_OPT) -c $<

shell: shell.o readline.o commande.o
	gcc $(GCC_OPT) -o $@ $^

shell.o: shell.c

readline.o : readline.c

commande.o : commande.c

test_commande: test_commande.o commande.o
	gcc $(GCC_OPT) -o $@ $^

clean:
	rm -f *.o *~ shell test_commande
