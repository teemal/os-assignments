#
# R Jesse Chaney
# 

CC      = gcc
DEBUG   = -g -ggdb
OPT     = -O3 -fno-math-errno
VERBOSE = -DALLOW_VERBOSE
DEFINES = -DDEBUG $(VERBOSE)
LDFLAGS = -lsqlite3

CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
	-Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
	-Wdeclaration-after-statement -Wunsafe-loop-optimizations \
	-Wuninitialized	$(DEFINES)

PROG1 = connect
SRCS1 = \
	$(PROG1).c \
	$(EMPTY)
HEADERS1 = \
	tests.h \
	$(EMPTY)
OBJS1 = $(SRCS1:.c=.o)

PROG2 = create_table
SRCS2 = \
	$(PROG2).c \
	$(EMPTY)
HEADERS2 = \
	tests.h \
	$(EMPTY)
OBJS2 = $(SRCS2:.c=.o)

PROG3 = insert
SRCS3 = \
	$(PROG3).c \
	$(EMPTY)
HEADERS3 = \
	tests.h \
	$(EMPTY)
OBJS3 = $(SRCS3:.c=.o)

PROG4 = select
SRCS4 = \
	$(PROG4).c \
	$(EMPTY)
HEADERS4 = \
	tests.h \
	$(EMPTY)
OBJS4 = $(SRCS4:.c=.o)

PROG5 = update
SRCS5 = \
	$(PROG5).c \
	$(EMPTY)
HEADERS5 = \
	tests.h \
	$(EMPTY)
OBJS5 = $(SRCS5:.c=.o)

PROG6 = delete
SRCS6 = \
	$(PROG6).c \
	$(EMPTY)
HEADERS6 = \
	tests.h \
	$(EMPTY)
OBJS6 = $(SRCS6:.c=.o)

PROG7 = select2
SRCS7 = \
	$(PROG7).c \
	$(EMPTY)
HEADERS7 = \
	tests.h \
	$(EMPTY)
OBJS7 = $(SRCS7:.c=.o)

PROG8 = update2
SRCS8 = \
	$(PROG8).c \
	$(EMPTY)
HEADERS8 = \
	tests.h \
	$(EMPTY)
OBJS8 = $(SRCS8:.c=.o)

PROG9 = insert2
SRCS9 = \
	$(PROG9).c \
	$(EMPTY)
HEADERS9 = \
	tests.h \
	$(EMPTY)
OBJS9 = $(SRCS9:.c=.o)

PROGS = $(PROG1) $(PROG2) $(PROG3) $(PROG4) $(PROG5) $(PROG6) $(PROG7) $(PROG8) $(PROG9)


all: $(PROGS)


$(PROG1): $(PROG1).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG1)

$(PROG1).o: $(PROG1).c $(HEADERS1)
	$(CC) $(CFLAGS) -c $<

$(PROG2): $(PROG2).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG2)

$(PROG2).o: $(PROG2).c $(HEADERS2)
	$(CC) $(CFLAGS) -c $<

$(PROG3): $(PROG3).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG3)

$(PROG3).o: $(PROG3).c $(HEADERS3)
	$(CC) $(CFLAGS) -c $<

$(PROG4): $(PROG4).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG4)

$(PROG4).o: $(PROG4).c $(HEADERS4)
	$(CC) $(CFLAGS) -c $<

$(PROG5): $(PROG5).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG5)

$(PROG5).o: $(PROG5).c $(HEADERS5)
	$(CC) $(CFLAGS) -c $<

$(PROG6): $(PROG6).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG6)

$(PROG6).o: $(PROG6).c $(HEADERS6)
	$(CC) $(CFLAGS) -c $<

$(PROG7): $(PROG7).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG6)

$(PROG7).o: $(PROG7).c $(HEADERS7)
	$(CC) $(CFLAGS) -c $<

$(PROG8): $(PROG8).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG6)

$(PROG8).o: $(PROG8).c $(HEADERS8)
	$(CC) $(CFLAGS) -c $<

$(PROG9): $(PROG9).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	chmod a+rx $(PROG6)

$(PROG9).o: $(PROG9).c $(HEADERS9)
	$(CC) $(CFLAGS) -c $<

# clean up the compiled files and editor chaff
clean cls:
	rm -f $(PROGS) *.o *~ \#*

ci:
	ci -m"Lazy Check-in" -l *.[ch] [mM]akefile
