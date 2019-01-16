/*	Rule stack administration.
*/

#include	<stdio.h>
#include	<stdlib.h>

#include	"rulestack.h"

#define	RS_SIZE		100

struct rule {
	char lhs;
	const char *rhs;
};

static struct rule rule_stack[RS_SIZE];
static int rSP;

void
push_rule(char lhs, const char *rhs) {
	struct rule *rs = &rule_stack[rSP++];

	if (rSP > RS_SIZE) {
		fprintf(stderr, "Rule stack overflow\n");
		exit(1);
	}

	rs->lhs = lhs;
	rs->rhs = rhs;
}

void
pop_rule(void) {
	--rSP;
}

void
print_derivation(void) {
	int r;

	for (r = 0; r < rSP; r++) {
		struct rule *rs = &rule_stack[r];

		printf("%c -> %s\n", rs->lhs, rs->rhs);
	}
	printf("\n");
}
