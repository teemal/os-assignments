CC = gcc -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes  -Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized -Wextra

all: bigFiles bennysh

bigFiles: bigFiles.o
	$(CC) -o bigFiles bigFiles.o

bigFiles.o: bigFiles.c
	$(CC) -c bigFiles.c

bennysh: bennysh.o
	$(CC) -o bennysh bennysh.o

bennysh.o: bennysh.c
	$(CC) -c bennysh.c

clean:
	rm -f *.o bigFiles bennysh *~ \#*

git:
	git add .
	git commit -m "add to git so you dont lose all your stuff and cry"
