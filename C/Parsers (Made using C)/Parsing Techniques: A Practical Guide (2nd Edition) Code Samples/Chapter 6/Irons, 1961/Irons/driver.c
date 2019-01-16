/*	Mr Irons uses
		to diagram	for	to parse
		components	for	right hand side
		subject		for	left hand side
		syntax unit	for	symbol
*/

#include	<stdio.h>
#include	<stdlib.h>

#include	"global.h"
#include	"print.h"
#include	"driver.h"

int TRAN[NSYNU];		/* from syntax unit to syntax table */
int STAB[GRAMSIZE];		/* the rules in the syntax table */
int STC[GRAMSIZE];		/* the links to further rules */
int SUCCR[NSYNU][NSYNU];	/* the successor in the searching process */
int STGOAL;			/* the start syntax unit */
int STCOMP;			/* the start components */

int OUTPUT[IOSIZE];

char *INPUT = input;

int stab_pos;
static int rule_cnt;

static void
error(const char *s) {
	printf("Rule #%d, ERROR: %s\n", rule_cnt, s);
	exit(1);
}

static void
to_STAB(int c) {
	if (stab_pos == GRAMSIZE)
		error("GRAMSIZE too small");
	STAB[stab_pos++] = c;
}

static void
init_grammar(void) {
/*	Irons parser, like any LC parser, requires both a goal and a rule
	it is working on; to start properly, it needs a starting rule rather
	than a starting symbol. We therefore create a dummy rule which
	consists of the start symbol only.
*/
	to_STAB(RIGHTBRACE);
	STGOAL = DUMMYSTART;
	STCOMP = stab_pos;
	to_STAB(start);
	to_STAB(DUMMYSTART);
	to_STAB(LEFTBRACE);
	to_STAB(RIGHTBRACE);
}

void
rule(	int subj,			/* the subject */
	char *comp			/* the components */
) {
	char *cp = comp;
	int *sp = &TRAN[*cp++];
		/* where the rule is going to be registered */

	rule_cnt++;
	while (*sp) {
		int s = *sp;

		while (STAB[s] == *cp && STAB[s+1] != LEFTBRACE) {
			s++, cp++;
		}
		if (!*cp) {
			error("rule is head of previous rule");
		}
		sp = &STC[s];
	}

	*sp = stab_pos;
	while (*cp) {
		to_STAB(*cp++);
	}
	to_STAB(subj);

	to_STAB(LEFTBRACE);
	to_STAB(subj);
	to_STAB('-');
	to_STAB('>');
	cp = comp;
	while (*cp) {
		to_STAB(*cp++);
	}
	to_STAB(RIGHTBRACE);

	SUCCR[*comp][subj] = 1;
}

static void
completeSUCCR(int w /* take the SUCCR[][] seriously */) {
	/* to Warshall or not to Warshall */
	int i;

	for (i = 1; i < NSYNU; i++) {
		int j;

		for (j = 1; j < NSYNU; j++) {
			if (!w) {
				/* just set all SUCCR[] */
				SUCCR[j][i] = 1;
			}
			else
			if (SUCCR[j][i]) {
				/* calculate transitive closure */
				int k;
				for (k = 1; k < NSYNU; k++) {
					if (SUCCR[i][k])
						SUCCR[j][k] = 1;
				}
			}
		}
	}
}

int
main(void) {
	int p_tree;

	init_grammar();
	grammar();
	print_grammar();

	completeSUCCR(1);

	print_input();
	if (DIAGRAM(STCOMP, STGOAL, &p_tree)) {
		printf("OK\n");
pp(p_tree);
		/* print the nonterminal in the dummy start rule */
		print_alt(OUTPUT[-p_tree]-3, p_tree-1);
		printf("\n");
	}
	else {
		printf("KO\n");
	}

	return 0;
}
