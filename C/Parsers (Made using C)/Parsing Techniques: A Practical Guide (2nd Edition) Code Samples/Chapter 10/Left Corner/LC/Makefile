#
#	Makefile for processing the grammar from Section 10.1.1 by LLgen
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

TEST =		book
TEST =		book_ext
TEST =		book_alt

test:		$(TEST)$(EXE) Makefile
		echo "n+n*n" | ./$(TEST)$(EXE)

$(TEST)$(EXE):	$(TEST).g dot.o
		LLgen $(TEST).g
		$(CC) Lpars.c $(TEST).c dot.o -o $@

lint:		Lpars.c $(TEST).c dot.c
		lint -ansi -xh Lpars.c $(TEST).c dot.c | grep -v LL

.PHONY:		clean fresh
clean:
		rm -f $(TEST) $(TEST)$(EXE) $(TEST).c \
			Lpars.[ch] LL.output *.o t core

fresh:		clean

Lpars.o:	dot.h
dot.o:		dot.h
