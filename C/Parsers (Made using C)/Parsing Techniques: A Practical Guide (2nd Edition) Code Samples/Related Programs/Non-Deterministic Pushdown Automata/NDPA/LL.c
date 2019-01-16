/*	This is a LL non-deterministic pushdown automaton
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
print_accepted(void) {
	int ln = strlen(text);
	int i;

	for (i = 0; i < ln-TP; i++) {
		putchar(' ');
	}
	for (i = 0; i < TP; i++) {
		putchar(text[i]);
	}
	putchar(' ');
}

static void
print_status(void) {
	int i;

	printf("t: ");
	print_accepted();
	for (i = TP; text[i]; i++) {
		putchar(text[i]);
	}
	printf("\n");

	printf("s: ");
	print_accepted();
	for (i = SP; i > 0; i--) {
		putchar(stack[i-1]);
	}
	printf("\n");
}

static void
NDPDA(void) {
	/* one step of the non-deterministic pushdown automaton */

	print_status();

	/* try to accept one symbol */
	if (SP >= 1 && stack[SP-1] == text[TP]) {
		/* shift it */
		--SP, TP++;
		if (SP == 0) {
			print_derivation();	/* parsing found */
		}
		else {
			NDPDA();
		}
		/* shift it back */
		SP++, --TP;
	}
	else {
		/* try to expand one lhs */
		grammar();
	}
}

void
rule(char lhs, const char rhs[]) {
	if (SP >= 1 && stack[SP-1] == lhs) {
		/* the rule applies */
		int rhs_len = strlen(rhs);
		int i;

		push_rule(lhs, rhs);

		/* pop lhs */
		--SP;

		/* push rhs, reversed */
		for (i = rhs_len; i > 0; i--) {
			if (SP == STACKSIZE) {
				fprintf(stderr, "Stack overlow\n");
				exit(1);
			}
			stack[SP++] = rhs[i-1];
		}

		NDPDA();

		/* pop rhs */
		SP -= rhs_len;
		
		/* push lhs */
		stack[SP++] = lhs;

		pop_rule();
	}
}

int
main(void) {
	/* prepare input */
	strcpy(text, input);		/* text := input# */
	strcat(text, "#");

	/* prepare stack */
	stack[SP++] = '#';		/* stack := start# */
	stack[SP++] = start;

	/* parse */
	NDPDA();

	return 0;
}
