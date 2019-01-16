#include		"grammar.h"
#include		"rule.h"

const char input[] = "((((((((()))))]]]]";

char start = 'S';

void
grammar(void) {
	rule('S', "A"	);
	rule('A', "(A]"	);
	rule('A', "B"	);
	rule('B', "(B)"	);
	rule('B', "()"	);
}

