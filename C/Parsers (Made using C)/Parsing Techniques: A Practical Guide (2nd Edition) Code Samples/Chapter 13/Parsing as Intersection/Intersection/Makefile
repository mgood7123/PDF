# ======== Makefile for CFG-FSA intersection according to Bar-Hillel ========
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
		@echo make test_cfg-fsa
		@echo make all

all:		test_cfg-fsa


GRAMMAR =	gram_Unger
GRAMMAR =	gramLR1
GRAMMAR =	gramLR1.try
GRAMMAR =	gramC

TEST_OPTIONS =	-r
TEST_OPTIONS =	-r -v
TEST_OPTIONS =	-v
TEST_OPTIONS =	-r
TEST_INPUT =	$(GRAMMAR)

# ==== CFG-FSA
test_cfg-fsa:	cfg-fsa$(EXE) $(TEST_INPUT)
		./cfg-fsa$(EXE) $(TEST_OPTIONS) $(TEST_INPUT)

CFG-FSA_SRC =	cfg-fsa.c grammar.c expand.c restore.c clean.c \
		read.c token.c free.c print.c error.c
CFG-FSA_OBJ =	cfg-fsa.o grammar.o expand.o restore.c clean.o \
		read.o token.o free.o print.o error.o
cfg-fsa$(EXE):	$(CFG-FSA_OBJ)
		$(CC) $(CFG-FSA_OBJ) -o $@

# ==== Auxilairies
figures:		

lint:		$(CFG-FSA_SRC) Makefile
		$(LINT) $(CFG-FSA_SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o core a.out *.?txt \
			cfg-fsa cfg-fsa$(EXE)

fresh:		clean

# DO NOT DELETE THIS LINE -- make depend depends on it.
cfg-fsa.o: cfg-fsa.h
cfg-fsa.o: grammar.h
cfg-fsa.o: read.h
cfg-fsa.o: expand.h
cfg-fsa.o: restore.h
cfg-fsa.o: free.h
cfg-fsa.o: print.h
cfg-fsa.o: error.h
clean.o: cfg-fsa.h
clean.o: grammar.h
clean.o: clean.h
clean.o: free.h
clean.o: print.h
error.o: read.h
error.o: error.h
expand.o: cfg-fsa.h
expand.o: grammar.h
expand.o: expand.h
expand.o: print.h
expand.o: clean.h
free.o: grammar.h
free.o: free.h
grammar.o: grammar.h
grammar.o: error.h
print.o: grammar.h
print.o: print.h
print.o: error.h
read.o: grammar.h
read.o: read.h
read.o: token.h
read.o: error.h
restore.o: cfg-fsa.h
restore.o: grammar.h
restore.o: restore.h
restore.o: clean.h
restore.o: print.h
token.o: grammar.h
token.o: token.h
token.o: error.h
