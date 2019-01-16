/*	This is a C version of the parser described in
	J. Eickel, M. Paul, F.L. Bauer, K. Samelson,
	A syntax-controlled generator of formal language processors,
	Commun. ACM, Vol. 6, no. 8, Aug 1963, 451-455.
*/

#include	<stdio.h>
#include	<stdlib.h>

#include	"global.h"
#include	"print.h"

int m;					/* # of productions of length 1 */
int n;					/* # of productions of length 2 */
char U[GRSIZE], V[GRSIZE];		/* productions of length 1 */
char R[GRSIZE], S[GRSIZE], T[GRSIZE];	/* productions of length 2 */

char M[NSYM];				/* set for all syntactic symbols */
char ALPHA[NSYM][NSYM];
char OMEGA[NSYM][NSYM];
char MU[NSYM][NSYM];

int rsize;				/* size of reducing trans. table */
char RTT[RTTSIZE][5];			/* reducing transition table */
#define	BELOW	0
#define	TOP	1
#define	AHEAD	2
#define	ACT	3
#define	LHS	4

static char X[XSIZE];			/* input in parser format */
static char PHI = '#';			/* demarcation */
static char Z;				/* start symbol */

static void
error(const char *s) {
	printf("ERROR: %s\n", s);
	exit(1);
}

void
rule(char lhs, const char *rhs) {
	if (rhs[1] == 0) {
		m++;
		if (m == GRSIZE)
			error("too many rules of length 1");
		U[m] = lhs; V[m] = rhs[0];
	}
	else
	if (rhs[2] == 0) {
		n++;
		if (n == GRSIZE)
			error("too many rules of length 2");
		R[n] = lhs; S[n] = rhs[0]; T[n] = rhs[1];
	}
	else {
		/* split up the rule, introducing an auxiliary symbol */
		static char aux_symbol = '0';
		char new_rhs[3];

		new_rhs[0] = rhs[0];
		new_rhs[1] = aux_symbol++;
		new_rhs[2] = '\0';
		rule(lhs, new_rhs);
		rule(new_rhs[1], &rhs[1]);
	}
}

/*	add_XX() has for X:
		c	class of symbols
		s	single symbols
*/

static int
triple(int b, int t, int a) {
	int r;

	for (r = 0; r < rsize; r++) {
		char *rtt = RTT[r];

		if (rtt[BELOW] == b && rtt[TOP] == t && rtt[AHEAD] == a) {
			return r;
		}
	}
	return r;
}

static void
add_ss(int b, int t, int a, int act, int lhs) {
	int r;
	char *rtt;
	
	if ('A' <= a && a <= 'Z')
		return;

	r = triple(b, t, a);
	rtt = RTT[r];
	if (r < rsize) {
		if (rtt[ACT] == act && rtt[LHS] == lhs)
			return;
		printf("%c%c%c -> %d(%c) at %d, rsize = %d: ",
			rtt[BELOW], rtt[TOP], rtt[AHEAD],
			rtt[ACT], rtt[LHS], r, rsize);
		error("duplicate RTT entry");
	}

	if (rsize == RTTSIZE)
		error("too many triples");
	rtt[BELOW] = b;
	rtt[TOP] = t;
	rtt[AHEAD] = a;
	rtt[ACT] = act;
	rtt[LHS] = lhs;
	rsize++;
}

static void
add_sc(int b, int t, const char *ac, int act, int lhs) {
	int y;

	for (y = 0; y < NSYM; y++) if (ac[y]) {
		add_ss(b, t, y, act, lhs);
	}
}

static void
add_cc(int b, const char *tc, const char *ac, int act, int lhs) {
	int y;

	for (y = 0; y < NSYM; y++) if (tc[y]) {
		add_sc(b, y, ac, act, lhs);
	}
}

static void
add_cs(int b, char *tc, int a, int act, int lhs) {
	int y;

	for (y = 0; y < NSYM; y++) if (tc[y]) {
		add_ss(b, y, a, act, lhs);
	}
}

static void
construct_M(void) {
	int i, j;

	for (j = 1; j <= m; j++) {
		M[U[j]] = 1;
		M[V[j]] = 1;
	}
	for (i = 1; i <= n; i++) {
		M[R[i]] = 1;
		M[S[i]] = 1;
		M[T[i]] = 1;
	}
}

static void
construct(char ALPHA[][NSYM], char S[]) {
	int e;
	int changed;

	/* (1) */
	for (e = 0; e < NSYM; e++) if (M[e]) {
		ALPHA[e][e] = 1;
	}

	do {
		int ay;

		changed = 0;
		for (ay = 0; ay <= NSYM; ay++) if (M[ay]) {
			char *Ya = ALPHA[ay];
			int Y;

			for (Y = 0; Y <= NSYM; Y++) if (Ya[Y]) {
				int j, i;
				/* (2) */
				for (j = 1; j <= m; j++) {
					if (U[j] == Y) {
						if (!Ya[V[j]]) {
							changed = 1;
							Ya[V[j]] = 1;
						}
					}
				}
				/* 3 */
				for (i = 1; i <= n; i++) {
					if (R[i] == Y) {
						if (!Ya[S[i]]) {
							changed = 1;
							Ya[S[i]] = 1;
						}
					}
				}
			}
		}
	} while (changed);
}

static void
construct_MU(void) {
	int j;
	int changed;

	/* 1 */
	for (j = 1; j <= m; j++) {
		MU[U[j]][V[j]] = 1;
	}

	do {
		int ay;

		changed = 0;
		for (ay = 0; ay <= NSYM; ay++) if (M[ay]) {
			char *Ya = MU[ay];
			int Y;

			for (Y = 0; Y <= NSYM; Y++) if (Ya[Y]) {
				/* (2) */
				for (j = 1; j <= m; j++) {
					if (U[j] == Y) {
						if (!Ya[V[j]]) {
							changed = 1;
							Ya[V[j]] = 1;
						}
					}
				}
			}
		}
	} while (changed);

	/* check */
	for (j = 1; j <= m; j++) {
		int y = U[j];

		if (MU[y][y]) {
			printf("%c: ", y);
			error("circular definition");
		}
	}
}

static void
construct_RTT(void) {
	int e;

	for (e = 1; e <= n; e++) {
		int i;

		/* I: */
		for (i = 1; i <= n; i++) if (OMEGA[S[i]][R[e]]) {
			add_cc(S[e], MU[T[e]], ALPHA[T[i]], 1, T[e]);
			add_sc(S[e], T[e], ALPHA[T[i]], 2, R[e]);
		}

		/* II: */
		for (i = 1; i <= n; i++) if (ALPHA[T[i]][R[e]]) {
			add_cc(S[i], MU[S[e]], ALPHA[T[e]], 1, S[e]);
			add_sc(S[i], S[e], ALPHA[T[e]], 3, '-');
		}

		/* III: */
		if (ALPHA[Z][R[e]]) {
			add_cc(PHI, MU[S[e]], ALPHA[T[e]], 1, S[e]);
			add_sc(PHI, S[e], ALPHA[T[e]], 3, '-');
		}

		/* IV: */
		if (OMEGA[Z][R[e]]) {
			add_cs(S[e], MU[T[e]], PHI, 1, T[e]);
			add_ss(S[e], T[e], PHI, 2, R[e]);
		}
	}

	/* V: */
	add_cs(PHI, MU[Z], PHI, 1, Z);
	add_ss(PHI, Z, PHI, 4, '-');		/* final state (DG) */
}

static void
read_input(void) {
	int p = 0;
	int i = 0;

	X[p++] = PHI;
	while (input[i]) {
		if (p == XSIZE)
			error("input too long");
		X[p++] = input[i++];
	}
	X[p] = PHI;
	printf("X: %s\n", X);
}

static void
parse(void) {
	int running = 1;
	int r = 1;
	int s = 2;

	while (running) {
		char *rtt = RTT[triple(X[r-1], X[r], X[s])];

		print_status(X, r, s);
		switch (rtt[ACT]) {
		case 0:
			printf("input is not a word for %c\n", Z);
			exit(1);
			break;
		case 1:
			printf("%c ::= %c\n", rtt[LHS], X[r]);
			X[r] = rtt[LHS];
			break;
		case 2:
			printf("%c ::= %c%c\n", rtt[LHS], X[r-1], X[r]);
			X[--r] = rtt[LHS];
			break;
		case 3:
			printf("shift %c\n", X[s]);
			X[++r] = X[s++];
			break;
		case 4:
			printf("OK\n");
			running = 0;
			break;
		}
	}
}

int
main(void) {
	Z = start;
	grammar();
	print_grammar();

	construct_M();
	construct(ALPHA, S);
	construct(OMEGA, T);
	construct_MU();
/*	print_tables();*/

	construct_RTT();
	print_RTTsize();
/*	print_RTT();*/

	read_input();
	parse();
	return 0;
}

