aall: linked_list

linked_list: linked_list.o
	gcc -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -o linked_list linked_list.o

linked_list.o: linked_list.c linked_list.h
	gcc -g -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -c linked_list.c

clean:
	rm -f *.o linked_list *~ \#*
