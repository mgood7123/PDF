/*	BC(2,1) parser driver; reads lines from input and parses them
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"table.h"		/* defines table and tabsize */

static char input[100];			/* input with read pointer */
static int rp;

static char stack[100];
static int top;

static int mark[1000];			/* to check usage of table entries */

static void
print_info(void) {
	int i;

	/* print stack */
	for (i = 0; i <= top; i++) {
		printf("%c ", stack[i]);
	}
	/* print gap */
	printf("\t\t");
	/* print rest of input */
	for (i = rp; input[i]; i++) {
		printf("%c ", input[i]);
	}
	printf("\n");
}

static void
error(const char *msg) {
	printf("BC(2,1): %s\n", msg);
	print_info();
	exit(1);
}

static void
shift(void) {
	stack[++top] = input[rp++];
}

static void
reduce(const struct entry *ep) {
	int i;
	int rhs_size = strlen(ep->rhs);

	/* check if right-hand side is indeed present */
	for (i = 0; i < rhs_size; i++) {
		if (stack[top-rhs_size+1+i] != ep->rhs[i]) {
			error("Reduce error");
		}
	}
	top -= rhs_size;
	stack[++top] = ep->lhs;
}

static int
parse_step(void) {
	int i;
	int tos_found = 0;		/* top of stack found */

	/* scan table */
	for (i = 0; i < tabsize; i++) {
		const struct entry *ep = &table[i];

		if (ep->l2 == stack[top-1] && ep->l1 == stack[top]) {
			tos_found = 1;
			if (ep->r == input[rp]) {
				/* entry found */
				mark[i]++;
				if (ep->lhs == SHIFT) {
					printf("Shift\n");
					shift();
					return 1;
				}
				else
				if (ep->lhs == ACCEPT) {
					printf("Accept\n\n");
					return 0;
				}
				if (ep->lhs == SERROR) {
					printf("Syntax error in input\n");
					print_info();
					return 0;
				}
				if (ep->lhs == IERROR) {
					fprintf(stderr, "%c%c%c\n",
						ep->l2, ep->l1, ep->r);
					error("Internal error");
					return 0;
				}
				else {
					printf("%c -> %s\n",
						ep->lhs, ep->rhs);
					reduce(ep);
					return 1;
				}
			}
		}
	}
	if (!tos_found) {
		/* illegal top of stack */
		error("No entry");
	}
	return 0;
}

static void
report_marks(void) {
	int i;

	for (i = 0; i < tabsize; i++) {
		const struct entry *ep = &table[i];
		if (!mark[i] && ep->lhs != IERROR) {
			printf("%c%c %c\n", ep->l2, ep->l1, ep->r);
		}
	}
}

int
main(void) {
	while (gets(input)) {
		/* initialize parser */
		rp = 0;
		top = -1;
		shift(); shift();

		/* repeat parse step */
		while (parse_step()) {
		}
	}
	report_marks();

	return 0;
}
