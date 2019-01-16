#	This file is part of the CS sentence generator CSgen
#	Written by Dick Grune, Vrije Universiteit, Amsterdam.
#	$Id: Makefile,v 1.5 2016-09-19 08:26:14 Gebruiker Exp $
#

# System dependencies
include		../../sysidf.mk

# Entry points:
test:

help:
	@echo  'Entry points:'
	@echo  'test		compile and run a simple test'
	@echo  ''
	@echo  'lint		check code'
	@echo  ''
	@echo  'clean		remove created files'
	@echo  'fresh		remove created + imported files'

# Compiler Options
C_OPT =		#-O2#		# C optimization level
LIBFLAGS =	#		# library flags
LINTFLAGS =	-h#		# lint flags

# Compilers: GNU
CC =		gcc
CFLAGS =	-D$(SYSTEM) -D$(SUBSYSTEM) $(C_OPT)
LINT =		lint -ansi -D$(SYSTEM) -D$(SUBSYSTEM)

# Parameters for testing 'CSgen'
## CSGEN_FLAGS describes the non-file parameters, CSGEN_INPUT the files
CSGEN_INPUT =   #

GRAM =	        gram0
CSGEN_FLAGS =   #

GRAM =	        gram1
CSGEN_FLAGS =   #

GRAM =	        gram2
CSGEN_FLAGS =   -r

GRAM =	        gram4
CSGEN_FLAGS =   -s#

test:	       	CSgen$(EXE) $(CSGEN_INPUT)
		./CSgen $(CSGEN_FLAGS) $(CSGEN_INPUT)

# Definition of 'CSgen'
CSGEN_SRC =	CSgen.c production.c $(GRAM).c grammar.c error.c #
CSGEN_OBJ =	CSgen.o production.o $(GRAM).o grammar.o error.o #
CSGEN_HDR =	        production.h    gram.h grammar.h error.h #
CSGEN_GRB +=	CSgen CSgen.exe

CSgen$(EXE):	$(CSGEN_OBJ) Makefile
		$(CC) $(CSGEN_OBJ) -o $@

# Miscellaneous
lint:		$(CSGEN_SRC) $(CSGEN_HDR)
		$(LINT) $(LINTFLAGS) $(CSGEN_SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o
		rm -f $(CSGEN_GRB)
		rm -f a.out core

fresh:		clean

# DO NOT DELETE THIS LINE -- make depend depends on it.
CSgen.o: CSgen.c gram.h grammar.h production.h
error.o: error.c error.h
gram0.o: gram0.c gram.h
gram1.o: gram1.c gram.h
grammar.o: grammar.c gram.h grammar.h error.h
production.o: production.c gram.h grammar.h production.h
