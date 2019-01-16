/* UBDA1 from paper */

#include	"global.h"

char input[] = "aaaa";

char start = 'A';

void
grammar(void) {
	rule('A', "AA");
	rule('A', "a");
}

