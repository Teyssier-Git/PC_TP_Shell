all: shell

GCC_OPT= -Wall -Werror -g

shell: shell.o readline.o
	gcc $(GCC_OPT) -o $@ $^

shell.o: shell.c readline.h
	gcc $(GCC_OPT) -c $<

readline.o: readline.c readline.h
	gcc $(GCC_OPT) -c $<

clean:
	rm -f *.o *~ shell
