CC=gcc
CFLAGS=-I.
LIB=-lm -Wno-stringop-overflow

serverBuild: calc.o switchCase.o server.o graphUtility.o
	$(CC) -o server calc.o switchCase.o server.o graphUtility.o ${LIB}

clientBuild: client/client.o
	$(CC) -o client/client client/client.o

serverRun: 
	./server

clientRun:
	./client/client