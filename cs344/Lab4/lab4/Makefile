CC = gcc -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized -Wextra

all: mystat csv2bin bin2csv 

mystat: mystat.o
	$(CC) -o mystat mystat.o

mystat.o: mystat.c file_struct.h
	$(CC) -c mystat.c

csv2bin: csv2bin.o
	$(CC) -o csv2bin csv2bin.o

csv2bin.o: csv2bin.c file_struct.h
	$(CC) -c csv2bin.c

bin2csv: bin2csv.o
	$(CC) -o bin2csv bin2csv.o

bin2csv.o: bin2csv.c file_struct.h
	$(CC) -c bin2csv.c

clean:
	rm -f *.o hellomake *~ \#* 
