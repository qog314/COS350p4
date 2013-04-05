#AUTHOR: James Tanner
#COURSE: COS 350 - Systems Programming
#PURPOSE: Program 4

scroll: scroll.o
	gcc scroll.o -o scroll

scroll.o: scroll.c
	gcc -c  scroll.c

clean:
	rm -f *.o
	rm -rf cos350/submit/*
