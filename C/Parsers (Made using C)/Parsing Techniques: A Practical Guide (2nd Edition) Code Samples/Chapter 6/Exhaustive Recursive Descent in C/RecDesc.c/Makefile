#
# Makefile for the Exhaustive Recursive Descent Parser from Chapter 6, in C
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

INPUT_TXT = 	aabc
INPUT_TXT = 	abcc
INPUT_TXT = 	abc#			# ambiguous

test:		parser$(EXE) Makefile
		echo $(INPUT_TXT) | ./parser$(EXE)

SRC =		parser.c 
OBJ =		parser.o

parser$(EXE):	$(OBJ)
		$(CC) -o $@ $(OBJ)

tex:		parser.i.tex parseraux.i.tex

.PHONY:		clean fresh
clean:
		rm -f parser.o parser parser.exe parser*.i.tex core

fresh:		clean

parser.i.tex: parser.i ../prog2tex
parseraux.i.tex: parseraux.i ../prog2tex
