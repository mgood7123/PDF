/*	This one will not work in any recursive descent parser.
*/

#include	"driver.h"

char input[] = "xxxxxx";

char start = 'S';

void
grammar(void) {
	rule('S', "A");
	/* causes the parser to gobble up a too large A */
	rule('A', "xAx");
	rule('A', "xx");
}
