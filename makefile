# AUTHOR: James Tanner
# COURSE: COS 350 - Systems Programming
# PURPOSE: Program 4

COMP=gcc
FLAGS=-g -pg

scroll: scroll.o
	$(COMP) $(FLAGS) scroll.o -o scroll

scroll.o: scroll.c
	gcc -c $(FLAGS) scroll.c

man: manpage
	man ./manpage
   
clean:
	rm -f *.o
