#	Variantions on: Algorithm W from Shyamasundar's
#	Precedence parsing using Domolki's algorithm
#	Intern. J. Comput. Math., A6, 1977, pp. 105-114
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

PARSE =		parse2#		# a structured rewrite
PARSE =		parse#		# the original algorithm, corrected

# Entry points:
test_parser:

help:
	@echo  'Entry points:'
	@echo  'test_parser	compile parser and run a simple test'
	@echo  'parser[.exe]	compile parser'
	@echo  ''
	@echo  'package.zip	create publication package'
	@echo  'lint		check sources'
	@echo  ''
	@echo  'clean		remove created files'
	@echo  'fresh		remove created + imported files'

# Compiler Options
C_OPT =		#-O2#		# C optimization level
LIBFLAGS =	#		# library flags
LINTFLAGS =	-h#		# lint flags

# Compilers: GNU + DJGPP v2
CC =		gcc
CFLAGS =	-pedantic -D$(SYSTEM) -D$(SUBSYSTEM) $(C_OPT)
LINT =		lint -ansi -D$(SYSTEM) -D$(SUBSYSTEM)

# Parameters for testing 'parser'
## PARSER_FLAGS describes the non-file parameters, PARSER_INPUT the files
PARSER_FLAGS =	"@p^p$$"
PARSER_FLAGS =	"@p+p*p+p$$"
PARSER_INPUT =	

test_parser:	parser$(EXE) $(PARSER_INPUT)
		./parser $(PARSER_FLAGS) $(PARSER_INPUT)

# Definition of 'parser'
PARSER_SRC =	$(PARSE).c vector.c grammar.c stack.c report.c parser.c error.c
PARSER_OBJ =	$(PARSE).o vector.o grammar.o stack.o report.o parser.o error.o
PARSER_HDR =	parse.h vector.h grammar.h stack.h report.h error.h
PARSER_GRB =	parser parser.exe

parser$(EXE):	$(PARSER_OBJ)
		$(CC) $(PARSER_OBJ) -o $@ $(LIBFLAGS)

# Publication Package
ZIPS =		src.zip
ALLFILES =	read_me comments $(ZIPS)
ALLSRC =	Makefile $(PARSER_SRC) $(PARSER_HDR) $(PARSER_INPUT)
ZIPS_GRB =	package.zip $(ZIPS)

package.zip:	$(ALLFILES)
		$(ZIP) -o package $(ALLFILES)

src.zip:	$(ALLSRC)
		$(ZIP) -o src $(ALLSRC)


# Miscellaneous
lint:		$(PARSER_SRC) $(PARSER_HDR)
		$(LINT) $(LINTFLAGS) $(PARSER_SRC)

.PHONY:		clean fresh
clean:
		rm -f *.o
		rm -f $(PARSER_GRB)
		rm -f $(ZIPS_GRB)
		rm -f a.out core

fresh:		clean
		rm -f $(LIB_GRB)

# DO NOT DELETE THIS LINE -- make depend depends on it.
grammar.o: vector.h
grammar.o: grammar.h
grammar.o: error.h
parse.o: vector.h
parse.o: grammar.h
parse.o: stack.h
parse.o: error.h
parse.o: parse.h
parse.o: pdebug.i
parse2.o: vector.h
parse2.o: grammar.h
parse2.o: error.h
parse2.o: parse.h
parse2.o: pdebug.i
parser.o: vector.h
parser.o: grammar.h
parser.o: parse.h
parser.o: report.h
report.o: vector.h
report.o: grammar.h
report.o: report.h
report.o: error.h
stack.o: vector.h
stack.o: error.h
stack.o: stack.h
vector.o: vector.h
vector.o: grammar.h
