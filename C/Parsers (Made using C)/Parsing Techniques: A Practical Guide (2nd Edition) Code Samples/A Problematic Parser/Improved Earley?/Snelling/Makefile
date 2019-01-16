# ======== Makefile for the O(n^2) CF parser according Snelling ========
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
		@echo make test_parser
		@echo make all

all:		test_parser

GRAMMAR =	gramLeo
GRAMMAR =	gram1
GRAMMAR =	gram3

TEST_OPTIONS =	
TEST_INPUT =	

# ==== PARSER
test_parser:	parser$(EXE) $(TEST_INPUT)
		./parser$(EXE) $(TEST_OPTIONS) $(TEST_INPUT)

PARSER_SRC =	parser.c grammar.c driver.c $(GRAMMAR).c
PARSER_OBJ =	parser.o grammar.o driver.o $(GRAMMAR).o
parser$(EXE):	$(PARSER_OBJ) Makefile
		$(CC) $(PARSER_OBJ) -o $@

# ==== Auxilairies
figures:		

lint:		$(PARSER_SRC) Makefile
		$(LINT) $(PARSER_SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o core a.out *.?txt \
			parser parser$(EXE)

fresh:		clean

# DO NOT DELETE THIS LINE -- make depend depends on it.

driver.o: global.h
driver.o: grammar.h
driver.o: driver.h
driver.o: parser.h
gram3.o: global.h
gramLeo.o: global.h
grammar.o: global.h
grammar.o: grammar.h
parser.o: global.h
parser.o: parser.h
parser.o: driver.h
parser.o: grammar.h
