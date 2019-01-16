# ======== Makefile for the LR(k,inf) parser according to Szymanski ========
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

GRAMMAR =	G8
GRAMMAR =	GTai
GRAMMAR =	GTaix
GRAMMAR =	Gmid
GRAMMAR =	G918
GRAMMAR =	Ggood

TEST_OPTIONS =	-k1 -v -a
TEST_INPUT =	
#LINT =		/opt/SUNWspro/bin/lint
#LINTFLAGS =	

# ==== PARSER
test_parser:	parser$(EXE) $(TEST_INPUT)
		./parser$(EXE) $(TEST_OPTIONS) $(TEST_INPUT)

PARSER_SRC =	parser.c itemset.c computeVk.c parse.c $(GRAMMAR).c
PARSER_OBJ =	parser.o itemset.o computeVk.o parse.o $(GRAMMAR).o
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

G3.o: itemset.h
G3.o: parser.h
G7.o: itemset.h
G7.o: parser.h
G8.o: itemset.h
G8.o: parser.h
GLR0.o: itemset.h
GLR0.o: parser.h
GLR20.o: itemset.h
GLR20.o: parser.h
Gleft.o: itemset.h
Gleft.o: parser.h
Gx.o: itemset.h
Gx.o: parser.h
computeVk.o: itemset.h
computeVk.o: parser.h
computeVk.o: computeVk.h
itemset.o: itemset.h
itemset.o: parser.h
parse.o: itemset.h
parse.o: parser.h
parse.o: computeVk.h
parse.o: parse.h
parser.o: itemset.h
parser.o: parser.h
parser.o: parse.h
