/*	Bad one */

char input[] = "aaa";

char start = 'S';

grammar(void) {
	rule('S', "LSa");
	rule('S', "a");
	rule('L', "");
}

