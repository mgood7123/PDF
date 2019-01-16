/*	This is gram2 + all strings that have 1 character wrong (i.e. ?) */

#include		"grammar.h"
#include		"rule.h"

const char input[] = "((((((((())))?]]]]";

char start = 'S';

void
grammar(void) {
	/*	Z is A with one error; Y is B with one error,
		for lack of multi-character nonterminal names
	*/
	rule('S', "A"	);
	rule('S', "Z"	);
	rule('A', "(A]"	);
	rule('Z', "?A]"	);
	rule('Z', "(Z]"	);
	rule('Z', "(A?"	);
	rule('A', "B"	);
	rule('Z', "Y"	);
	rule('B', "(B)"	);
	rule('Y', "?B)"	);
	rule('Y', "(Y)"	);
	rule('Y', "(B?"	);
	rule('B', ""	);
}

