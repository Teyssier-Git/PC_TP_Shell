all: shell

GCC_OPT= -Wall -Werror -g

*.o: $^
	gcc $(GCC_OPT) -c $<

shell: shell.o readline.o commande.o
	gcc $(GCC_OPT) -o $@ $^

shell.o: shell.c readline.h commande.h

readline.o: readline.c readline.h

commande.o: commande.c


clean:
	rm -f *.o *~ shell
