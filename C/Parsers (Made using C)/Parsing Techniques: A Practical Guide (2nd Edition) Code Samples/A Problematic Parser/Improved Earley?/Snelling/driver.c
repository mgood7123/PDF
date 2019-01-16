#include	"global.h"
#include	"grammar.h"
#include	"driver.h"
#include	"parser.h"

char X[MAX_n+1];			/* the input */
int n;					/* its length */

static void
get_input(void) {
	int i;

	for (i = 0; input[i]; i++) {
		X[i+1] = input[i];
	}
	n = i;
}

static void
modify_grammar(void) {
	D[0] = D0;
	C[0][1] = start;
	C[0][2] = end;
	bar[0] = 2;
}

int
main(void) {
	grammar();
	modify_grammar();
	print_grammar();

	get_input();
	X[n+1] = end;

	recognizer();
	return 0;
}
