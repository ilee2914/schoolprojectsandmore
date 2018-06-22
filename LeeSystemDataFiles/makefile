CC=gcc
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: $(OBJS)
	$(CC) uidmain.o -o uida.out
	$(CC) gidmain.o -o gida.out
clean:
	rm *.out *.o