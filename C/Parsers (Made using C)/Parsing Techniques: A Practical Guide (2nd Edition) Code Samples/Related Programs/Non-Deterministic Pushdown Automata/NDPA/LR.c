/*	This is a LR non-deterministic pushdown automaton
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"global.h"
#include	"grammar.h"
#include	"rulestack.h"
#include	"rule.h"

static char stack[STACKSIZE];
static int SP;

static char text[TEXTSIZE];
static int TP;

static void
print_status(void) {
	int ln = strlen(text);
	int i;

	for (i = 0; i < ln-SP; i++) {
		putchar(' ');
	}
	for (i = 0; i < SP; i++) {
		putchar(stack[i]);
	}
	putchar(' ');
	for (i = TP; text[i]; i++) {
		putchar(text[i]);
	}
	printf("\n");
}

static void
NDPDA(void) {
	/* one step of the non-deterministic pushdown automaton */
	char symbol_shifted;

	print_status();

	/* try shifting one symbol */
	symbol_shifted = stack[SP++] = text[TP++];
	if (symbol_shifted == '#') {
		if (stack[0] == start && stack[1] == '#') {
			print_derivation();	/* parsing found */
		}
	}
	else {
		NDPDA();
	}
	/* shift it back */
	--SP, --TP;

	/* try to reduce one rhs */
	grammar();
}

void
rule(char lhs, const char rhs[]) {
	int rhs_len = strlen(rhs);

	if (SP >= rhs_len && strncmp(&stack[SP-rhs_len], rhs, rhs_len) == 0) {
		int i;
		/* the rule applies */

		push_rule(lhs, rhs);

		/* pop rhs */
		SP -= rhs_len;

		/* push lhs */
		if (SP == STACKSIZE) {
			fprintf(stderr, "Stack overlow\n");
			exit(1);
		}
		stack[SP++] = lhs;

		NDPDA();

		/* pop lhs */
		--SP;

		/* push rhs */
		for (i = 0; rhs[i]; i++) {
			stack[SP++] = rhs[i];
		}

		pop_rule();
	}
}

int
main(void) {
	/* prepare input */
	strcpy(text, input);		/* text := input# */
	strcat(text, "#");

	/* prepare stack */
	;				/* stack := */

	/* parse */
	NDPDA();

	return 0;
}
