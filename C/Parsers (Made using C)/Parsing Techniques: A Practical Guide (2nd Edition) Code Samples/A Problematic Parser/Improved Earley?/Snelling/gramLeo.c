/* email Joop Leo */

#include	"global.h"

char input[] = "aaaaabaaaaa";

char start = 'S';

void
grammar(void) {
	rule('S', "aSa");
	rule('S', "Ab");
	rule('A', "aA");
	rule('A', "a");
}

