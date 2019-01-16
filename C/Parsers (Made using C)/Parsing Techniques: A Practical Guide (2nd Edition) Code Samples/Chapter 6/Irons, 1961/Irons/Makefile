# ======== Makefile for Iron's parser, CACM 4(1)1961, 51-55 ========
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

GRAM =		gram1
DIAGRAM =	DIAGRAM
DIAGRAM =	DIAGRAM.goto

all:		Irons$(EXE)
		Irons$(EXE)

SRC =		$(DIAGRAM).c driver.c print.c $(GRAM).c
OBJ =		$(DIAGRAM).o driver.o print.o $(GRAM).o

Irons$(EXE):	$(OBJ) Makefile
		$(CC) $(OBJ) -o $@

lint:		$(SRC)
		$(LINT) $(SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o Irons$(EXE) a.out core

fresh:		clean

# DO NOT DELETE

DIAGRAM.o: global.h
DIAGRAM.o: driver.h
DIAGRAM.goto.o: global.h
driver.o: global.h
driver.o: print.h
driver.o: driver.h
gram4.o: driver.h
print.o: global.h
print.o: driver.h
print.o: print.h
