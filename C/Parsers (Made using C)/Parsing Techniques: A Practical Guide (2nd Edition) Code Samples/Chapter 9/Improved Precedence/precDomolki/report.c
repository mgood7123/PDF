#include	"vector.h"
#include	"grammar.h"
#include	"report.h"
#include	"error.h"

#include	<stdio.h>
#include	<string.h>

#define	PARSE_SIZE	100

static int parse[PARSE_SIZE];
static int parse_length = 0;

void
AddProductionToParse(int m) {
	if (parse_length == PARSE_SIZE) error("parse too long to be stored");
	parse[parse_length++] = m;
}	

void
PrintProduction(int m) {
	printf("Production %d: %c -> %s\n", m, LHS(m), RHS(m));
}

void
PrintParse(void) {
	int i;
	
	for (i = 0; i < parse_length; i++) {
		PrintProduction(parse[i]);
	}
}

static int next_prod;

static void
pr_token(char t, int level) {
	static pr_pos;
	
	if (pr_pos > level*3) {
		printf("\n");
		pr_pos = 0;
	}
	
	while (pr_pos < level*3) {
		printf(" ");
		pr_pos++;
	}
	printf("%c", t);
	pr_pos++;
}

static void
pr_parse_tree(char lhs, int level) {
	pr_token(lhs, level);
	if (is_nonterminal(lhs)) {
		/* print subtree */
		int m = (	next_prod == 0
			?	(error("parse exhausted prematurely"), 0)
			:	parse[--next_prod]
			);
		const char *rhs =
			(	lhs != LHS(m)
			?	(char *)(error("non-matching parse"), 0)
			:	RHS(m)
			);
		int i;

		for (i = strlen(rhs); i > 0; i--) {
			pr_parse_tree(rhs[i-1], level+1);
		}
	}
}

void
PrintParseTree(void) {
	next_prod = parse_length;
	pr_parse_tree(S, 0);
}
