#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>

#include	"itemset.h"
#include	"parser.h"
#include	"parse.h"

int v_level = 0;
int a_flag = 0;
int i_flag = 0;
#define	OPTIONS	"[-kN] [-tN] [-i(tem test)] [-v(erbose) [-v(ery)]] [-a(lternative format)]"

int k = 1;
int t = -1;

/*	Szymanski [1973] writes: "all our parsing methods degenerate
	to the LR(0) method when this value (i.e. 0) is chosen for k."
	(page 43).  That may be true, but the algorithm does not.

	This has been remedied by slightly modifying the algorithm, as
	explainded in the computation of V3 in computeVk.c.
*/

int is_Symbol(char c) {
	/* reserve all braces for future reg. expressions as right context */
	return !(c == '{' || c == '}' || c == '[' || c == ']' || c == '|' ||
		 isdigit((unsigned)c)
	);
}

int is_bracket(char c) {
	return c == ']';
}

static Symbol
bad_symbol_in(struct rule *r) {
	Symbol *RHS = r->RHS;

	if (!is_Symbol(r->LHS)) return r->LHS;
	while (RHS[0]) {
		if (!is_Symbol(RHS[0])) return RHS[0];
		RHS++;
	}
	return '\0';
}

static void
check_grammar(void) {
	int i;

	for (i = 0; grammar[i].LHS; i++) {
		Symbol bad = bad_symbol_in(&grammar[i]);

		if (bad) {
			fprintf(stderr,
				"rule %d contains the non-grammar symbol %c\n",
				i, bad
			);
			exit(1);
		}
	}
	/* check for left recursion or implement regexps for right context */
}

static void
explain_call(const char *arg) {
	fprintf(stderr, "Unknown option to LRkInf: %s\n", arg);

	printf("LRkInf Parser according to Szymanski; options are:\n");
	printf("-kN	look-ahead length (default = 1)\n");
	printf("-tN	by-pass maximum (default = inf)\n");
	printf("-v [-v]	verbosity level (default = 0)\n");
	printf("-a	print alternative format (full items rather than brackets) \n");
	printf("-i	show items for a sentential form rather than parse sentence\n");
	printf("\n");
}

int
main(int argc, char *argv[]) {
	int i;

	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'k':
			k = atoi(&argv[1][2]);
			break;
		case 't':
			t = atoi(&argv[1][2]);
			break;
		case 'v':
			v_level++;
			break;
		case 'a':
			a_flag = 1;
			break;
		case 'i':
			i_flag = 1;
			break;
		default:
			explain_call(argv[1]);
			exit(1);
		}
		argc--, argv++;
	}

	if (t != -1) {
		fprintf(stderr, "LR(k,t) not implemented\n");
		exit(1);
	}

	check_grammar();
	for (i = 0; input[i]; i++) {
		printf("Input = `%s'\n", input[i]);
		if (i_flag) {
			test_sentential_form(input[i]);
		}
		else {
			parse_sentence(input[i]);
		}
	}

	return 0;
}
