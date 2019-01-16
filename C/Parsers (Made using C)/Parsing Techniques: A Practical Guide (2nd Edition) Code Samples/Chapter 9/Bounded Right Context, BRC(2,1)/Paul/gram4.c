/*	This is the unrepaired, ambiguous if-then-else grammar:
		S -> iSeS | iS | a
	which is correctly rejected.
*/

#include	"global.h"

char input[] = "iiiaea";

char start = 'S';

void
grammar(void) {
	rule('S', "iSeS");
	rule('S', "iS");
	rule('S', "a");
}

