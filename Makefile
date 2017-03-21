CC = gcc
CFLAGS = -Wall -ansi -pedantic -O2
LFLAGS = -lm
OUT = ep1
IN = ep1.c
OBJS = ep1.o StringOps.o Pilha.o

all: $(OUT)

clean: 
	rm -f $(OBJS) $(OUT)

$(OUT): $(OBJS) 
	$(CC) $(OBJS) $(CFLAGS) -o $(OUT)


ep1.o: $(IN)
	$(CC) $(IN) $(CFLAGS) -c -o ep1.o

StringOps.o: StringOps.c
	$(CC) StringOps.c $(CFLAGS) -c -o StringOps.o

Pilha.o: Pilha.c
	$(CC) Pilha.c $(CFLAGS) -c -o Pilha.o
