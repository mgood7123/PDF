#
#		Makefile for Rytter Parser in Java
#

N =		1

RESULT =	res$(N)

SRC =		Grammar.java Symbol.java Rule.java Sentence.java CykItem.java \
		RytterItem.java Rytter.java

# using @ feature because parameter list is too long for MSDOS
OPTIONS =	@srctmp
REQUIRED =	srctmp

all:		test

test:		$(SRC) $(REQUIRED)
		javac $(OPTIONS)
		java Rytter UserGrammar$(N) < UserInput$(N)

compile:	$(SRC) $(REQUIRED)
		javac $(OPTIONS)

srctmp:		Makefile
		echo $(SRC) > srctmp

compare:	compile Makefile $(RESULT)
		java Rytter UserGrammar$(N) < UserInput$(N) | tee newres | diff -h - $(RESULT)

compareAll:	# Java-style goal
		make N=1 compare
		make N=2 compare
		make N=3 compare
		make N=4 compare
		make N=5 compare
		make N=6 compare

lint:		$(SRC) $(REQUIRED)
		javac -Xlint $(OPTIONS)

.PHONY:		clean fresh
clean:
		rm -f *.class srctmp newres core

fresh:		clean
