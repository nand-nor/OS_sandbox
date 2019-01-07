#Allie Clifford
#Makefile for comp111 assignment 3
#Date created: 10/24/2018
#Last modified: 11/7/2018

CC = gcc
CLFAGS = -g -std=c99 -Wall -Wextra -Werror -Wfatal-errors -pedantic
LDLIBS = -lrt -lpthread


all: watch shadow.so proc 1.out 1b.out 2.out 2b.out 3.out 3b.out 4.out 4b.out 5.out 5b.out 6.out

proc: proc.o procTest.o
	$(CC) $^ -o $@ $(LDLIBS)

watch: watch.o
	$(CC) $^ -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

shadow.so: shadow.c
	gcc -g -shared -fPIC shadow.c -o shadow.so -ldl

##########TEST FILES########
1.out: 1.c
	gcc -g -o 1.out 1.c
2.out: 2.c
	gcc -g -o 2.out 2.c
3.out: 3.c 
	gcc -g -o 3.out 3.c
4.out: 4.c
	gcc -g -o 4.out 4.c -lpthread
5.out: 5.c
	gcc -g -o 5.out 5.c
6.out: 6.c
	gcc -g -o 6.out 6.c
1b.out: 1b.c
	gcc -g -o 1b.out 1b.c
2b.out: 2b.c
	gcc -g -o 2b.out 2b.c
3b.out: 3b.c 
	gcc -g -o 3b.out 3b.c
4b.out: 4b.c
	gcc -g -o 4b.out 4b.c -lpthread
5b.out: 5b.c
	gcc -g -o 5b.out 5b.c


clean:
	rm -f *.o *.out watch *.so

