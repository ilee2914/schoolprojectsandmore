CC=gcc
CFLAGS=-c -Wall -std=c99

all: main1 main2

main1: main1.o
	$(CC) main1.o -o main1.out

main2: main2.o
	$(CC) main2.o -o main2.out

main1.o: main1.c
	$(CC) $(CFLAGS) main1.c

main2..o: main2.c
	$(CC) $(CFLAGS) main2.c
	
clean:
	rm *.out *.o