#
#		Makefile for parsers in Prolog, using CProlog
#

CLAUSE =	an.cpr
CLAUSE =	anbncn
CLAUSE =	canc_eps
CLAUSE =	canc_expr
CLAUSE =	mbtest
CLAUSE =	neder
CLAUSE =	gram1
CLAUSE =	gram2

# causes all answers to be printed
test:		$(CLAUSE).cpr
		echo "['$(CLAUSE).cpr']. try. halt." |\
		cprolog

.PHONY:		clean fresh
clean:

fresh:		clean
