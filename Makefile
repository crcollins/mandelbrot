CFLAGS=-g -Wall -Wstrict-prototypes -pedantic -std=c99
CC=gcc $(CFLAGS)
MPICC=mpicc $(CFLAGS)

all: mandelbrot buddhabrot

util.o: util.c util.h
	$(CC) -c util.c

mandelbrot.o: mandelbrot.c util.h
	$(CC) -c mandelbrot.c

mandelbrot: mandelbrot.o util.o
	$(CC) -O3 mandelbrot.o util.o -o mandelbrot -lm 

buddhabrot.o: buddhabrot.c util.h
	$(MPICC) -c buddhabrot.c

buddhabrot: buddhabrot.o util.o
	 $(MPICC) buddhabrot.o util.o -o buddhabrot -lm

clean:
	rm *.o *.ppm mandelbrot buddhabrot 
