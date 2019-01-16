# ======== Makefile for Non-Deterministic Push-Down automata ========
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
		@echo "make TYPE=[LL/LR] GRAM=gramX test"
		@echo make test

# ==== PARSER
TYPE =	LR
GRAM =	gram4

test:		$(TYPE)
		$(TYPE)

PARSER_SRC =	rulestack.c $(TYPE).c $(GRAM).c
PARSER_OBJ =	rulestack.o $(TYPE).o $(GRAM).o

$(TYPE):	$(PARSER_OBJ) Makefile
		$(CC) $(PARSER_OBJ) -o $@


# ==== Auxilairies
figures:		

lint:		$(PARSER_SRC) Makefile
		$(LINT) $(PARSER_SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o core a.out *.?txt \
			LL LL$(EXE) LR LR$(EXE)

fresh:		clean

# DO NOT DELETE THIS LINE -- make depend depends on it.
