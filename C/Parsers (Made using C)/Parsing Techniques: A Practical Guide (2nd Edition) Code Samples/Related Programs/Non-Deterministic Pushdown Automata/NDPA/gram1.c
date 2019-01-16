#include		"grammar.h"
#include		"rule.h"

const char input[] = "xxxxxx";

char start = 'A';

void
grammar(void) {
	rule('A', "xAx");
	rule('A', "xx");
}

