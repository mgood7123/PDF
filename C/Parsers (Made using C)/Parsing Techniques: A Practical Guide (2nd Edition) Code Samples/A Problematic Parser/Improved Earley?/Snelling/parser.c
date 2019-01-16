/* This is a naive implementation of Snelling's algorithm, ICS 1973 */

#include	<stdio.h>
#include	<stdlib.h>

#include	"global.h"
#include	"parser.h"
#include	"driver.h"
#include	"grammar.h"

static int trace = 1;

							/* ACCESS ROUTINES */
static int
in_N(char n) {
	int i;

	for (i = 0; i < n_N; i++) {
		if (N[i] == n) return 1;
	}
	return 0;
}

static int
in_T(char n) {
	return !in_N(n);		/* text has: "&& n != end" */
}

							/* RECOGNIZER */
struct state {
	int in_use;
	int p;		/* rule number */
	int j;		/* position of dot */
	int f;		/* start position */
	int deleted_at;	/* i at which deleted, > 0 */
#ifdef	LONG
	int F[MAX_n+1];	/* list of item start positions */
	int n_F;	/* number of elements in F */
#endif
};

static struct state
state(int p, int j, int f) {
	struct state res;

	res.in_use = 1;
	res.p = p;
	res.j = j;
	res.f = f;
	res.deleted_at = 0;
	
	return res;
}

static int
are_equal_core_states(struct state s1, struct state s2) {
	return (s1.p == s2.p && s1.j == s2.j);
}

static int
are_equal_states(struct state s1, struct state s2) {
	return (are_equal_core_states(s1, s2) && s1.f == s2.f);
}

static basic_step_counter;

static char *
state2string(struct state s) {
	static char res[MAX_j + 100];
	char *p = res;
	int i;
	int deleted = 0;

	if (!s.in_use) {
		p += sprintf(p, "unused state");
		return res;
	}

	if (s.p < 0) {
		deleted = 1;
		s.p = -s.p;	/* it's a copy anyway */
	}

	p += sprintf(p, "%s%c -> ", (deleted ? "[" : " "), D[s.p]);
	for (i = 0; i < bar[s.p]; i++) {
		if (i == s.j) p += sprintf(p, ".");
		p += sprintf(p, "%c", C[s.p][i+1]);
	}
	if (i == s.j) p += sprintf(p, ".");
	i += 4;
	while (i < MAX_j-2) {
		p += sprintf(p, " ");
		i++;
	}
	p += sprintf(p, "@ %d", s.f);
	if (deleted) {
		p += sprintf(p, "] %d", s.deleted_at);
	}
	return res;
}

static char marked[128];		/* non-terminal markers */

static void
unmark_all_N(void) {
	int p;

	for (p = 0; p < 128; p++) {
		marked[p] = 0;
	}
}

static struct state S[MAX_n+1][MAX_s];

static void
insert_state_before(struct state s, struct state *set) {
	while (set->in_use) {
		struct state tmp = *set;
		*set++ = s;
		s = tmp;
	}
	*set = s;
}

static void
add_state(const char *process, struct state s, struct state *set) {
	/* since the algorithm is sensitive to the order of the items in S[i],
	   we keep items with the same core together, ordered by decreasing f
	*/
	if (trace) {
		printf("%s added state %s to S[%d]\n",
		       process, state2string(s), (set - S[0]) / MAX_s);
	}
	basic_step_counter++;
	while (set->in_use) {
		if (are_equal_core_states(s, *set)) {
			while (	set->in_use
			&&	are_equal_core_states(s, *set)
			) {
				if (s.f > set->f) {
					insert_state_before(s, set);
					return;
				}
				if (s.f == set->f) return;
				if (s.f < set->f) {
					set++;
				}
			}
			insert_state_before(s, set);
			return;
		}
		set++;
	}
	*set = s;
}

static void
dont_add_state(const char *process, struct state s, struct state *set) {
	if (trace) {
		printf("%s did not add state %s to S[%d]\n",
		       process, state2string(s), (set - S[0]) / MAX_s);
	}
}

static void
print_S(void) {
	int i;

	for (i = 0; S[i][0].in_use; i++) {
		int j;

		printf("S[%d]:\n", i);
		for (j = 0; S[i][j].in_use; j++) {
			printf("\t%s\n", state2string(S[i][j]));
		}
	}
}

static void
PREDICTOR(int i, int p, int j, int f) {
	if (!marked[C[p][j+1]]) {
		int r;
		
		for (r = 0; r <= d; r++) {
			if (D[r] == C[p][j+1]) {
				add_state("PREDICTOR  ", state(r,0,i), S[i]);
				marked[C[p][j+1]] = 1;
			}
		}
	}
}

static void
SCANNER(int i, int p, int j, int f) {
	if (C[p][j+1] == X[i+1]) {
		add_state("SCANNER    ", state(p, j+1, f), S[i+1]);
	}
}

static int
in_S(int i, int p, int j, int f) {
	int s;

	for (s = 0; S[i][s].in_use; s++) {
		if (are_equal_states(state(p, j, f), S[i][s])) return 1;
	}
	return 0;		
}

static struct state *SbpF[MAX_s];

static void fill_SbpF_from(int p, int f);

static void
add_to_SbpF(struct state *s) {
	int i;

	/* see if it is already there */
	for (i = 0; SbpF[i]; i++) {
		if (are_equal_states(*s, *SbpF[i])) return;
	}
	SbpF[i++] = s;
	SbpF[i++] = 0;
}

static void
fill_SbpF_from(int p, int i) {
	/* Add all items of the form A -> ... .B ... @ f with B = D[p] to
	   SbpF, in decreasing f order.
	   Rather than sorting them for f, we generate them for f = i, i-1, ...
	   which is stupid but safe.
	*/
	int f;

	for (f = i; f >= 0; f--) {
		struct state *s;
		
		for (s = &S[i][0]; s->in_use; s++) {
			if (	/* it has the starting point we want */
				s->f == f
			&&	/* it has the dot in front of the right NT */
				C[s->p][s->j+1] == D[p] && in_N(D[p])
				/* text has: C[p][j+1] == D[p] && in_N(D[p]) */
			) {
				add_to_SbpF(s);
			}
		}
	}
}

static void
print_SbpF(const char *msg) {
	int i;

	printf("SbpF %s:\n", msg);
	for (i = 0; SbpF[i]; i++) {
		printf("\t%s\n", state2string(*SbpF[i]));
	}
}

/* set of states added to S[i] by this call of the completer: */
static int n_Si; /* points to first free in S[i] at start of completer */

static void
begin_added_to_S_i(int i) {
	for (n_Si = 0; S[i][n_Si].in_use; n_Si++) {
	}
}

static int
added_to_S_i_by_this_completer_call(int i, int p1, int j1, int f1) {
	int s;

	for (s = n_Si; S[i][s].in_use; s++) {
		if (are_equal_states(state(p1, j1, f1) , S[i][s]))
			return 1;
	}
	return 0;
}

static void
print_added_to_S_i(int i) {
	int s;

	printf("added_to_S_i:\n");
	for (s = n_Si; S[i][s].in_use; s++) {
		printf("\t%s\n", state2string(S[i][s]));
	}
}

static int
condition_1(int i, int p1, int j1, int f1) {
	int f2;

	for (f2 = f1+1; f2 <= i; f2++) {
		if (added_to_S_i_by_this_completer_call(i,p1,j1+1,f2))
			return 1;
	}
	return 0;
}

static int
condition_3(int i, int p1, int j1, int f1) {
	int s;

	printf("3)\n"); fflush(stdout); fflush(stderr);
	
	for (s = 0; S[i][s].in_use; s++) {
		int p2 = S[i][s].p;
		int j2 = S[i][s].j;
		int f2 = S[i][s].f;

		if (j2 == bar[p2] && f2 == f1 && D[p1] == D[p2]) return 1;
	}
	printf("3)0\n"); fflush(stdout); fflush(stderr);
	return 0;
}

static void
delete_state(const char *process, struct state *s, int i) {
	/* The delete action should not shift the states in S[i], since S[i] is
	   being scanned by the overall loop.  So we temporarily destroy the
	   state and compress afterwards.
	*/
	if (trace) {
		printf("%s deleted state %s\n", process, state2string(*s));
	}
	/* make state unrecognizable */
	s->p = -s->p;	/* the state with p == 0 is never deleted */
	s->deleted_at = i;
}

static void
possibilities_1_to_5(int i, struct state *s1) {
	int p1 = s1->p;
	int j1 = s1->j;
	int f1 = s1->f;

	/* we pass on the pointer to s1, since we may have to delete s1 */
	if (trace) {
		printf("referring back to %s in SbpF\n",
		       state2string(state(p1, j1, f1)));
	}

#define	_EARLEY
#ifndef	EARLEY
	printf("A\n"); fflush(stdout); fflush(stderr);
	/* 1) */
	if (	j1+1 == bar[p1]		/* text has: bar[p] */
	&&	condition_1(i, p1, j1, f1)
	) {
		/* there is right recursivity */
		delete_state("COMPLETER.1", s1, i);
		dont_add_state("COMPLETER.1", state(p1, j1+1, f1), S[i]);
		return;
	}

	printf("B\n"); fflush(stdout); fflush(stderr);
	/* 2) */
	if (in_S(i, p1, j1+1, f1)) {
		printf("B1\n"); fflush(stdout); fflush(stderr);
		print_added_to_S_i(i);
		if (added_to_S_i_by_this_completer_call(i, p1, j1+1, f1)) {
			printf("B2\n"); fflush(stdout); fflush(stderr);
			/* there is an ambiguity */
			delete_state("COMPLETER.2", s1, i);
		}
		dont_add_state("COMPLETER.2", state(p1, j1+1, f1), S[i]);
		return;
	}

	printf("C\n"); fflush(stdout); fflush(stderr);
	/* 3) */
	if (	j1+1 == bar[p1]		/* text has: bar[p] */
	&&	condition_3(i, p1, j1, f1)
	) {
		printf("C1\n"); fflush(stdout); fflush(stderr);
		/* there is an ambiguity */
		dont_add_state("COMPLETER.3", state(p1, j1+1, f1), S[i]);
		return;
	}

	printf("D\n"); fflush(stdout); fflush(stderr);
	/* 4) */
	/* f-lists not implemented, so this reduces to case 5) */
#endif

	printf("E\n"); fflush(stdout); fflush(stderr);
	/* 5) */
	add_state("COMPLETER.5", state(p1, j1+1, f1), S[i]);
	if (j1+1 == bar[p1]) {
		fill_SbpF_from(p1, f1);
	}
}

static void
start_completer(int i) {
	begin_added_to_S_i(i);
	SbpF[0] = 0;
}

static void
end_completer(void) {
	/* clean up deleted items */
	/* not really needed ???? */
}

static void
COMPLETER(int i, int p, int j, int f) {
	int s;

	start_completer(i);
	if (trace) {
		printf("offered to COMPLETER at %d: %s\n",
		       i, state2string(state(p, j, f)));
	}

	/* for all states in SbpF */
	fill_SbpF_from(p, f);
	print_SbpF("after first entry");
	for (s = 0; SbpF[s]; s++) {
		printf("state in SbpF: %s\n", state2string(*SbpF[s]));
		possibilities_1_to_5(i, SbpF[s]);
	}
	print_SbpF("at end");
	end_completer();
}

static int
i_such_that_Si_is_empty(void) {
	int i;

	for (i = 0; i <= MAX_n; i++) {
		if (!S[i][0].in_use) return i;
	}
	return MAX_n+1;	/* cannot happen */
}

void
recognizer(void) {
	int i;
	int s;

	/* 3.1.1. Initialisation */
	add_state("INITIALISER", state(0, 0, 0), S[0]);
	unmark_all_N();
	i = 0;
	basic_step_counter = 0;

	/* 3.1.2. */
 E:	/* for every state <p,j,f> in S[i] */
	printf("\nstarting processing S[%d]:\n", i);
	for (s = 0; S[i][s].in_use; s++) {
		int p = S[i][s].p;
		int j = S[i][s].j;
		int f = S[i][s].f;

		if (j != bar[p] && in_N(C[p][j+1])) {
			PREDICTOR(i, p, j, f);
		}

		if (j != bar[p] && in_T(C[p][j+1])) {
			SCANNER(i, p, j, f);
		}

		if (j == bar[p]) {
			COMPLETER(i, p, j, f);
		}

		/*
		printf("after i = %d, s = %d\n", i, s);
		print_S();
		*/
	}
	unmark_all_N();
	printf("ending processing S[%d]\n", i);
	i = i+1;
	if (i <= n+1) goto E;

	/* 3.1.3. Termination */
	if (are_equal_states(S[n+1][0], state(0, 2, 0))) {
		printf("`%s' in L(G)\n", &X[1]);
	}
	i = i_such_that_Si_is_empty();
	if (0 <= i && i <= n+1) {
		printf("`%s' not in L(G), error at pos. %d\n", &X[1], i);
	}
	printf("n = %d, basic_step_counter = %d\n", n, basic_step_counter);
	print_S();
}
