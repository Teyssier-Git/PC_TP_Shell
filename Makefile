all:

GCC_OPT= -Wall

readline.o: readline.c
	gcc $(GCC_OPT) -c @<

clean:
	rm -f *.o *~
