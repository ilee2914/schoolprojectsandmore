CC=gcc
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

all: $(OBJS)
	$(CC) pathalloc.o error.o main.o -o a.out

clean:
	rm *.out *.o