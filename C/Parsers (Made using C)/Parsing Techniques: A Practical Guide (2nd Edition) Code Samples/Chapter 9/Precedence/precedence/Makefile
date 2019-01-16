#	This file is part of the parser generator Precedence.
#	Written by Dick Grune, Vrije Universiteit, Amsterdam.
#	$Id: Makefile,v 1.6 2007/12/22 21:12:01 dick Exp $
#

# System dependencies
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

# Entry points:
test_parser:

help:
	@echo  'Entry points:'
	@echo  'test_parser	compile parser and run a simple test'
	@echo  'test_table	compile table generator and run a simple test'
	@echo  'test_tparser	compile table-based parser and run a simple test'
	@echo  'grammar.i	generate the precedence table'
	@echo  ''
	@echo  'package.zip	create publication package'
	@echo  'lint		check sources'
	@echo  ''
	@echo  'clean		remove created files'
	@echo  'fresh		remove created + imported files'

GRAM =		gram1

# Compiler Options
C_OPT =		-Wall #-O2#		# C optimization level
LIBFLAGS =	#		# library flags
LINTFLAGS =	-h#		# lint flags

# Compilers: GNU + DJGPP v2
CC =		gcc
CFLAGS =	-pedantic -D$(SYSTEM) -D$(SUBSYSTEM) $(C_OPT)
LINT =		lint -ansi -D$(SYSTEM) -D$(SUBSYSTEM)

# The common modules
COMMON_SRC =	sets.c error.c
COMMON_OBJ =	sets.o error.o
COMMON_HDR =	sets.h error.h

# Parameters for testing 'parser'
## PARSER_FLAGS describes the non-file parameters, PARSER_INPUT the files
PARSER_FLAGS =  
PARSER_INPUT =  

test_parser:	parser$(EXE) $(PARSER_INPUT)
		./parser $(PARSER_FLAGS) $(PARSER_INPUT)

# Definition of 'parser'
PARSER_SRC =	parser.c precedence.c $(GRAM).c $(COMMON_SRC)
PARSER_OBJ =	parser.o precedence.o $(GRAM).o $(COMMON_OBJ)
PARSER_HDR =	precedence.h grammar.h $(COMMON_HDR)
PARSER_GRB =	parser parser.exe

parser$(EXE):	$(PARSER_OBJ)
		$(CC) $(PARSER_OBJ) -o $@

# Parameters for testing 'table'
## TABLE_FLAGS describes the non-file parameters, TABLE_INPUT the files
#TABLE_FLAGS =	-s
TABLE_INPUT =   

test_table:	table$(EXE) $(TABLE_INPUT)
		./table $(TABLE_FLAGS) $(TABLE_INPUT)

# Definition of 'table'
TABLE_SRC =	table.c precedence.c $(GRAM).c $(COMMON_SRC)
TABLE_OBJ =	table.o precedence.o $(GRAM).o $(COMMON_OBJ)
TABLE_HDR =	precedence.h grammar.h $(COMMON_HDR)
TABLE_GRB =	table table.exe

table$(EXE):	$(TABLE_OBJ)
		$(CC) $(TABLE_OBJ) -o $@

grammar.i:	$(GRAM).c ./table$(EXE)
		make test_table >prec.tmp
		rm prec.tmp
		./table$(EXE) $(TABLE_FLAGS) >$@

# Parameters for testing 'tparser'
## TPARSER_FLAGS describes the non-file parameters, TPARSER_INPUT the files
TPARSER_FLAGS = 
TPARSER_INPUT = 

test_tparser:	tparser$(EXE) $(TPARSER_INPUT)
		./tparser $(TPARSER_FLAGS) $(TPARSER_INPUT)

# Definition of 'tparser'
TPARSER_SRC =	parser.c table_prec.c $(COMMON_SRC)
TPARSER_OBJ =	parser.o table_prec.o $(COMMON_OBJ)
TPARSER_HDR =	precedence.h $(COMMON_HDR)
TPARSER_GRB =	tparser tparser.exe

tparser$(EXE):	$(TPARSER_OBJ)
		$(CC) $(TPARSER_OBJ) -o $@

# Publication Package
ZIPS =		src.zip dosexe.zip
ALLFILES =	readme.1st read_me parser.1 package.txt $(ZIPS)
ALLSRC =	Makefile $(PARSER_SRC) $(PARSER_HDR) $(PARSER_INPUT)
DOSEXE =	parser.exe table.exe tparser.exe
ZIPS_GRB =	package.zip $(ZIPS)

package.zip:	$(ALLFILES)
		$(ZIP) -o package $(ALLFILES)

src.zip:	$(ALLSRC)
		$(ZIP) -o src $(ALLSRC)

dosexe.zip:	$(DOSEXE)
		$(ZIP) -o dosexe $(DOSEXE)

# Miscellaneous
lint:		$(PARSER_SRC) $(PARSER_HDR) \
		$(TABLE_SRC) $(TABLE_HDR) \
		$(TPARSER_SRC) $(TPARSER_HDR) grammar.i
		$(LINT) $(LINTFLAGS) $(PARSER_SRC)
		$(LINT) $(LINTFLAGS) $(TABLE_SRC)
		$(LINT) $(LINTFLAGS) $(TPARSER_SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o
		rm -f $(PARSER_GRB)
		rm -f $(TABLE_GRB)
		rm -f $(TPARSER_GRB)
		rm -f grammar.i
		rm -f $(ZIPS_GRB)
		rm -f a.out core

fresh:		clean

# DO NOT DELETE THIS LINE -- make depend depends on it.

error.o: error.h
gram1.o: grammar.h
parser.o: precedence.h
parser.o: error.h
precedence.o: sets.h
precedence.o: grammar.h
precedence.o: error.h
sets.o: sets.h
sets.o: error.h
table.o: sets.h
table.o: grammar.h
table.o: precedence.h
table_prec.o: precedence.h
table_prec.o: error.h
table_prec.o: grammar.i
