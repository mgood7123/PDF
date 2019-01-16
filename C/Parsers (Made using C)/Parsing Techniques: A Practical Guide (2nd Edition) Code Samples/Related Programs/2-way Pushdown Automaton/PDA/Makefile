#	This file is part of the combined PDA interpreter PDA.
#	Written by Dick Grune, Vrije Universiteit, Amsterdam.
#	$Id: Makefile,v 1.3 2007/12/22 16:47:30 dick Exp $
#

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

help:
		@echo Calls are:
		@echo make test_pda
		@echo make all

all:		test_pda


# ==== PDA
TABLE =		LALRtable
PDA_OPTIONS =	<E
PDA_OPTIONS =	-v <LALRinput

TABLE =		palin
PDA_OPTIONS =	-v <palin_input2
PDA_OPTIONS =	-v <palin_input

test_pda:	pda$(EXE)
		./pda$(EXE) $(PDA_OPTIONS)

PDA_SRC =	pda.c $(TABLE).c
PDA_OBJ =	pda.o $(TABLE).o
pda$(EXE):	$(PDA_OBJ) Makefile
		$(CC) $(PDA_OBJ) -o $@

# ==== Auxilairies
figures:		

lint:		$(PDA_SRC) Makefile
		$(LINT) $(PDA_SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o core a.out *.?txt *.tmp \
			pda pda$(EXE)

fresh:		clean
		rm -f Doc.ps

# DO NOT DELETE THIS LINE -- make depend depends on it.

LALRtable.o: table.h
pda.o: table.h
