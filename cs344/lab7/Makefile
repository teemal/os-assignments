CC = gcc -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes  -Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized -Wextra

all: bennysh

bennysh: bennysh.o
	$(CC) -o bennysh bennysh.o

bennysh.o: bennysh.c
	$(CC) -c bennysh.c

clean:
	rm -f *.o bennysh *~ \#*

git:
	git add .
	git commit -m "add to git so you dont lose all your stuff and cry"
