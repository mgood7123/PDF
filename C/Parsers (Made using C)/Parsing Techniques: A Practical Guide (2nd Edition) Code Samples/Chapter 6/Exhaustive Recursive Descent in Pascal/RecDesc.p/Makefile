#
# Makefile for the Exhaustive Recursive Descent Parser from Chapter 6 in Pascal
# Uses the ACK Pascal compiler, but should work with any Pascal compiler
#


ACK =		/usr/local/ack


INPUT_TXT = 	aabc
INPUT_TXT = 	abcc
INPUT_TXT = 	abc#			# ambiguous

test:		parser
		echo $(INPUT_TXT) | parser

SRC =		parser.p 
OBJ =		parser.o

$(OBJ):		parser.p Makefile
		$(ACK)/bin/apc -c parser.p

parser:		$(OBJ)
		$(ACK)/bin/apc $(OBJ) -o parser

test_int:	parser.p Makefile
		$(ACK)/bin/em44 parser.p
		echo $(INPUT_TXT) | $(ACK)/bin/int

tex:		parser.p.tex

.PHONY:		clean fresh
clean:
		rm -f parser.o parser parser.p.tex parser.k e.out \
			core int.mess int.log

fresh:		clean

parser.p.tex: parser.p ../prog2tex
