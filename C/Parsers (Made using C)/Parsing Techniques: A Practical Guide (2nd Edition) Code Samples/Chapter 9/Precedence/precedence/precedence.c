/*	This file is part of the parser generator Precedence.
	Written by Dick Grune, Vrije Universiteit, Amsterdam.
	$Id: precedence.c,v 1.3 2002/10/18 12:33:43 dick Exp $
*/

#include	<stdio.h>
#include	<string.h>

#include	"sets.h"
#include	"grammar.h"
#include	"error.h"

#define	streq(s1,s2)	(strcmp(s1, s2) == 0)

static const char *
rhs(const char *rule[], int i) {
	/* lhs : rhs(0) rhs(1) ... rhs(-2) rhs(-1) 0 */

	/* rule[2] must exist */
	if (rule[2] == 0) error("grammar", "RHS empty");

	if (i >= 0) return rule[i+2];

	/* find end of rhs */
	while (rule[0]) {
		rule++;
	}
	/* and index from there */
	return rule[i];
}

enum direction {First, Last};

static void
add_precedence_operator(
	enum direction dir,
	const char *lhs,
	const char *operator_set[],
	const char *done[]
) {
	/* direction-blind programming ... */
	int i;

	if (item_in_set(lhs, done)) return;

	for (i = 0; grammar[i][0]; i++) {
		const char **rule = grammar[i];

		if (streq(rule[0], lhs)) {
			const char *rhs_end =		/* left or right end */
				rhs(rule, dir == First ? 0 : -1);

			if (is_terminal(rhs_end)) {
				add_item_to_set(rhs_end, operator_set);
			}
			else {	/* rhs_end is a non-terminal */
				const char *rhs_prox =	/* near l. or r. end */
					rhs(rule, dir == First ? 1 : -2);

				add_item_to_set(lhs, done);
				add_precedence_operator(
					dir, rhs_end, operator_set, done
				);
				remove_item_from_set(lhs, done);

				if (is_terminal(rhs_prox)) {
					add_item_to_set(rhs_prox, operator_set);
				}
			}
		}
	}
}

static void
first_operator(const char *lhs, const char *operator_set[]) {
	const char *done[MAX_SET_SIZE];

	operator_set[0] = 0;
	done[0] = 0;			/* nothing done yet */
	add_precedence_operator(First, lhs, operator_set, done);
}

static void
last_operator(const char *lhs, const char *operator_set[]) {
	const char *done[MAX_SET_SIZE];

	operator_set[0] = 0;
	done[0] = 0;			/* nothing done yet */
	add_precedence_operator(Last, lhs, operator_set, done);
}

static int
for_each_junction(
	int test(const char *m1, const char *m2, const char *m3,
		const char *t1, const char *t2
	),
	const char *t1, const char *t2
) {
	int i;

	for (i = 0; grammar[i][0]; i++) {
		const char **rule = grammar[i];
		int j;

		for (j = 2; rule[j+1]; j++) {
			const char *m1 = rule[j];	/* exists */
			const char *m2 = rule[j+1];	/* exists */
			const char *m3 = rule[j+2];	/* may not exist */

			if (is_terminal(m1) || is_terminal(m2)) {
				if (test(m1, m2, m3, t1, t2)) return 1;
			}
			else {
				error(	"grammar",
					"two consecutive nonterminals"
				);
			}
		}
	}
	return 0;
}

static int
prec_equal(
	const char *m1, const char *m2, const char *m3,
	const char *t1, const char *t2
) {
	if (!is_terminal(m1) || !streq(m1, t1)) return 0;
	if (is_terminal(m2)) return streq(m2, t2);
	return is_terminal(m3) && streq(m3, t2);
}

static int
prec_less(
	const char *m1, const char *m2, const char *m3,
	const char *t1, const char *t2
) {
	const char *operator_set[MAX_SET_SIZE];
	int i;

	if (!is_terminal(m1) || !streq(m1, t1)) return 0;
	if (is_terminal(m2)) return 0;

	first_operator(m2, operator_set);
	for (i = 0; operator_set[i]; i++) {
		if (streq(operator_set[i], t2)) return 1;
	}
	return 0;
}

static int
prec_greater(
	const char *m1, const char *m2, const char *m3,
	const char *t1, const char *t2
) {
	const char *operator_set[MAX_SET_SIZE];
	int i;

	if (is_terminal(m1)) return 0;
	if (!is_terminal(m2) || !streq(m2, t2)) return 0;

	last_operator(m1, operator_set);
	for (i = 0; operator_set[i]; i++) {
		if (streq(operator_set[i], t1)) return 1;
	}
	return 0;
}

#ifdef	XXXX				/* some tests */

static void
print_set(const char *msg, char *set[]) {
	int i;

	printf("%s:", msg);
	for (i = 0; set[i]; i++) {
		printf(" %s", set[i]);
	}
	printf("\n");
}

void
tests(void) {
	char *operator_set[MAX_SET_SIZE];
	const char *hash = "'#'";
	const char *semicolon = "';'";

	first_operator("assignment_list", operator_set);
	print_set("FIRST_OP", operator_set);

	last_operator("assignment_list", operator_set);
	print_set("LAST_OP", operator_set);

	printf("precedence('#', '#') = %c\n", precedence(hash, hash));
	printf("precedence(';', ';') = %c\n", precedence(semicolon, semicolon));
}

#endif

char
precedence(const char *t1, const char *t2) {
	int prec_lt = for_each_junction(prec_less, t1, t2);
	int prec_eq = for_each_junction(prec_equal, t1, t2);
	int prec_gt = for_each_junction(prec_greater, t1, t2);

	if (prec_lt + prec_eq + prec_gt > 1) {
		char msg[256];
		
		sprintf(msg, "precedence conflict: %s %s%s%s %s",
			t1,
			(prec_lt ? "<" : ""),
			(prec_eq ? "=" : ""),
			(prec_gt ? ">" : ""),
			t2
		);
		error("grammar", msg);
	}

	return (prec_eq ? '=' : prec_lt ? '<' : prec_gt ? '>' : ' ');
}
