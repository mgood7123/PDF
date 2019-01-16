#include	<stdio.h>

#include	"global.h"
#include	"driver.h"
#include	"print.h"

void
print_grammar(void) {
	int p;

	printf("STGOAL = %c, STCOMP = %d\n", STGOAL, STCOMP);
	for (p = 0; p < NSYNU; p++) {
		if (TRAN[p])
			printf("%c=>%d;", p, TRAN[p]);
	}
	printf("\n");

	for (p = 0; STAB[p]; p++) {
		printf("%c", STAB[p]);
	}
	printf("\n");

	for (p = 0; STAB[p]; p++) {
		printf("%d", STC[p]);
	}
	printf("\n");
}

void
print_input(void) {
	int p;

	for (p = 0; INPUT[p]; p++) {
		printf("%c", INPUT[p]);
	}
	printf("\n");
}

static void
print_rule(int rl) {
	while (STAB[rl] != RIGHTBRACE) {
		printf("%c", STAB[rl++]);
	}
}

void
pp(int p_tree) {
	int p, max;

	printf("p_tree = %d\n", p_tree);
	for (max = IOSIZE-1; max > 0; max--) {
		if (OUTPUT[max])
			break;
	}
	for (p = 0; p <= max; p++) {
		int op = OUTPUT[p];

		printf("[%d]", p);
		if (op > 0) {
			print_rule(op);
			printf(" ");
		}
		else	printf("%d ", OUTPUT[p]);
	}
	printf("\n");
}

#define	subject(s)	('A' <= s && s <= 'Z')

static void print_tree(char synu, int strt);

void
print_alt(int rl, int strt) {
	if (STAB[rl] == RIGHTBRACE) {
		/* we seem to have reached the head of the components */
		int i;

		/* is this a continuation of a previous rule? */
		for (i = 0; i < stab_pos; i++) {
			if (STC[i] == rl+1) {
				/* if so, continue there */
				print_alt(i-1, strt);
				return;
			}
		}

		/* is this an entry from the TRAN table? */
		for (i = 0; i < NSYNU; i++) {
			if (TRAN[i] == rl+1) {
				/* if so, print the first component */
				print_tree(i, strt);
				return;
			}
		}

		/* otherwise it must be the initial syntax unti */
		if (rl+1 != STCOMP)
			printf("UNKNOWN SYNTAX UNIT");
	}
	else {
		/* print previous components first */
		print_alt(rl-1, strt-1);
		/* then print this one */
		print_tree(STAB[rl], OUTPUT[-strt]);
	}
}

static void
print_tree(char synu, int strt) {
	if (subject(synu)) {
		if (strt < 0) {
			int rl = OUTPUT[-strt];

			if (rl) {
				print_rule(rl);
				printf(":(");
				rl -= 2;
				print_alt(rl-1, strt-1);
				printf(")");
			}
			else	printf("?");
		}
		else	printf("<%d>", strt);
	}
	else {
		printf("'%c'", synu);
	}
}
