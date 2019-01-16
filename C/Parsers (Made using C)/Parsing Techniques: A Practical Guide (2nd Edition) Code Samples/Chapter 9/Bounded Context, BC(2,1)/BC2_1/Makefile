# ======== Makefile for a BC2-1 parser ========
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

TEST_OPTIONS =	<input
TEST_INPUT =	

# ==== BC2-1
test_parser:	parser$(EXE) $(TEST_INPUT)
		./parser$(EXE) $(TEST_OPTIONS) $(TEST_INPUT)

BC2-1_SRC =	main.c table.c
BC2-1_OBJ =	main.o table.o
parser$(EXE):	$(BC2-1_OBJ)
		$(CC) $(BC2-1_OBJ) -o $@

# ==== Auxilairies
figures:		

lint:		$(BC2-1_SRC) Makefile
		$(LINT) $(BC2-1_SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o core a.out *.?txt \
			parser parser$(EXE)

fresh:		clean

# DO NOT DELETE THIS LINE -- make depend depends on it.
