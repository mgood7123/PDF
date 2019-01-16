#
# Makefile for the Partitioned LL(1) parser from Chapter 10 (Friede, 1979)
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

TEST =		anbcn

test:		$(TEST)$(EXE) Makefile
		echo "aaaccc" | ./$(TEST)$(EXE)

$(TEST)$(EXE):	$(TEST).o driver.o
		$(CC) $(TEST).o driver.o -o $@

lint:		$(TEST).c driver.c
		lint -ansi -xh $(TEST).c driver.c

.PHONY:		clean fresh
clean:
		rm -f $(TEST) $(TEST)$(EXE) \
			*.o t core

fresh:		clean

anbcn.o: anbcn.h
anbcn.o: driver.h
driver.o: anbcn.h
driver.o: driver.h
