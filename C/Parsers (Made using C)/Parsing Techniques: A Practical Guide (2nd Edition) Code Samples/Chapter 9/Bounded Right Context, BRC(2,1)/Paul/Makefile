# ======== Makefile for the CF parser of Eickel, CACM 6(8)1963,451-455 ========
#			set UNIX/DOS dependency here
# GENERAL
CC =		gcc
CFLAGS =	-ansi -Wstrict-prototypes -Wmissing-prototypes \
			-Wmissing-declarations -Wredundant-decls

# DOS version
EXE =		.exe
LEX =		flex
LINT =		echo No lint under MSDOS
TIME =		time
SLEEP =		rem
SYSTEM =	MSDOS
SUBSYSTEM =	MSDOS

# UNIX version
EXE =		#
LEX =		lex
LINT =		lint -ansi -xh
TIME =		time
SLEEP =		sleep
SYSTEM =	UNIX
SUBSYSTEM =	UNIX
#			end of UNIX/DOS dependency

GRAM =		gram4
GRAM =		gram0

test:		parser$(EXE)
		parser$(EXE)

SRC =		parser.c print.c $(GRAM).c
OBJ =		parser.o print.o $(GRAM).o

parser$(EXE):	$(OBJ)
		$(CC) $(OBJ) -o $@

lint:		$(SRC)
		$(LINT) $(SRC)

.PHONY:		clean fresh
clean:
		rm -f a.out core *.o parser$(EXE)

fresh:		clean

parser.o: global.h
print.o: global.h
