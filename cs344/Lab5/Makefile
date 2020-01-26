CC = gcc -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized -Wextra

all: bin2index binlookup beavar

bin2index: bin2index.o
	$(CC) -o bin2index bin2index.o

bin2index.o: bin2index.c file_struct.h
	$(CC) -c bin2index.c

binlookup: binlookup.o
	$(CC) -o binlookup binlookup.o

binlookup.o: binlookup.c file_struct.h
	$(CC) -c binlookup.c

beavar: beavar.o
	$(CC) -o beavar beavar.o

beavar.o: beavar.c beavar.h
	$(CC) -c beavar.c

clean:
	rm -f *.o bin2index binlookup beavar *~ \#*

git:
	git add .
	git commit -m "add to git so you dont lose all your stuff and cry"
