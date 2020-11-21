all: shell

GCC_OPT= -Wall -g

*.o: $^
	gcc $(GCC_OPT) -c $<

shell: shell.o readline.o commande.o
	gcc $(GCC_OPT) -o $@ $^

test_commande: test_commande.o commande.o
	gcc $(GCC_OPT) -o $@ $^

clean:
	rm -f *.o *~ shell test_commande
