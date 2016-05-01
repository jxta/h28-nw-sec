CC=gcc
#LFLAG=-lsocket -lnsl
CFLAG=

all:	server client

server: server.o
	$(CC) -o $@ server.o $(LFLAG)
server.o: server.c
	$(CC) $(CFLAG) -c $^ $(INCLUDE)

client: client.o
	$(CC) -o $@ client.o $(LFLAG)
client.o: client.c
	$(CC) $(CFLAG) -c $^ $(INCLUDE)

clean:
	@rm -f *.o *~ server client

