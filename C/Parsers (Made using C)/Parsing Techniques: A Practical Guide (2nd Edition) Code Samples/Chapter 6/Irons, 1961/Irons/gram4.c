/*	This is an epsilon-free version of the LL(inf) grammar
		S -> A
		A -> ( A ]  |  B
		B -> ( B )  |  eps
*/

#include	"driver.h"

char input[] = "((((((((()))))]]]]";

char start = 'S';

void
grammar(void) {
	rule('S', "A");
	rule('A', "(A]");
	rule('A', "(]");
	rule('A', "B");
	rule('B', "(B)");
	rule('B', "()");
}
