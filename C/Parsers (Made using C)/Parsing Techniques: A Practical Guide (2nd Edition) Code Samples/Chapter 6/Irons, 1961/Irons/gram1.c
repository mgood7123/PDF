#include	"driver.h"

char input[] = "(a+b)+a";

char start = 'E';

void
grammar(void) {
	rule('E', "E+T");
	rule('E', "T");

	rule('T', "T*F");
	rule('T', "F");

	rule('F', "a");
	rule('F', "b");
	rule('F', "(E)");
}

