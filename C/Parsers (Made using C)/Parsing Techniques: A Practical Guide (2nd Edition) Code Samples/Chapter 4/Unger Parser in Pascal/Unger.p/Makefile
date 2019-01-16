#
#		Makefile for Top-Down Unger Parser in Pascal
#

TYPE =		E# excludes WFST
TYPE =		P# includes WFST
RESULT =	res4$(TYPE)


TD_parserE.p:	TD_parser.gen
		<TD_parser.gen grep -v '^+' | sed 's/^.//' >$@

TD_parserP.p:	TD_parser.gen
		<TD_parser.gen grep -v '^-' | sed 's/^.//' >$@

TD_diff.p:	TD_parser.gen
		<TD_parser.gen grep '^+' | sed 's/^.//' >$@

compare:	TD_parser$(TYPE) Makefile $(RESULT)
		TD_parser$(TYPE) | tee newres | cmp - $(RESULT)

newres:		TD_parser$(TYPE) Makefile
		TD_parser$(TYPE) >newres

test(TYPE):	TD_parser$(TYPE).p Makefile
		/usr/local/ack/bin/em44 TD_parser$(TYPE).p
		/usr/local/ack/bin/int

SRC =		TD_parser$(TYPE).p 
OBJ =		TD_parser$(TYPE).o

TD_parser$(TYPE):$(OBJ)
		$(CC) $(OBJ) -o TD_parser$(TYPE)

tex:		TD_parserE.p.tex TD_diff.p.tex

lint:
		lint $(SRC)

.PHONY:		clean fresh
clean:
		rm -f *.[ok] TD_parser? TD_parser?.p TD_diff.p \
			TD_parser?.p.tex TD_diff.p.tex \
			[ae].out newres core int.mess int.log

fresh:		clean

TD_parserE.p.tex: TD_parserE.p ../prog2tex
TD_diff.p.tex: TD_diff.p ../prog2tex
