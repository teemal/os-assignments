CC = gcc -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized

all: socket_server socket_client

socket_server: socket_server.o
	$(CC) -o socket_server socket_server.o

socket_server.o: socket_server.c
	$(CC) -c socket_server.c

socket_client: socket_client.o
	$(CC) -o socket_client socket_client.o

socket_client.o: socket_client.c
	$(CC) -c socket_client.c

clean:
	rm -f *.o socket_server socket_client  *~ \#*

git:
	git add .
	git commit -m "add to git so you dont lose all your stuff and cry"
