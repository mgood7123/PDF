#include	"global.h"

char input[] = "p";

char start = 'A';

void
grammar(void) {
	rule('A', "B");
	rule('B', "C");
	rule('C', "p");
}
